#!/bin/bash
# =============================================================================
# run_bench.sh —— 阶段八 8.6 压测与优化对比脚本
#
# 作用：自动依次以不同「静态文件发送策略」(readwrite / mmap) 启动 http-server，
#       对同一静态文件接口施压，采集 QPS，最后输出前后对比表。
#       压测工具：探测所有可用工具(wrk/ab/webbench/curl)。
#         - 交互式运行且探测到多个时，弹出菜单让你手动选一个；
#         - 非交互(无 tty，如 CI/管道)或指定 TOOL= 时，按优先级 wrk > ab > webbench > curl 取第一个可用。
#       strace / perf 若存在则可选采集系统调用与 CPU 热点。
#
# 用法：
#   bash bench/run_bench.sh                     # 用默认参数跑 readwrite vs mmap（交互时可选工具）
#   STRATEGIES="readwrite" bash bench/run_bench.sh   # 只跑基线
#   BIN=/path/to/http-server CONNECTIONS=200 DURATION=15 bash bench/run_bench.sh
#   TOOL=ab bash bench/run_bench.sh             # 强制用 ab，跳过交互菜单
#   DO_STRACE=1 DO_PERF=1 bash bench/run_bench.sh    # 额外采集 strace/perf（需已安装）
#
# 可用环境变量（均有默认值）：
#   BIN          http-server 可执行文件路径（默认自动探测 build/http-server，缺失则尝试构建）
#   TOOL         强制指定压测工具(wrk/ab/webbench/curl)，须为已探测到的可用工具；不指定则交互选择或按优先级回退
#   STRATEGIES   要对比的策略列表，空格分隔（默认 "readwrite mmap"）
#   URI          压测的静态文件路径（默认 /index.html）
#   CONNECTIONS  并发连接数（wrk/ab/webbench 用，默认 100）
#   DURATION     压测持续秒数（wrk/webbench/curl 回退用，默认 10）
#   REQUESTS     总请求数（ab/curl 回退用，默认 20000）
#   THREADS      wrk 线程数（默认 4）
#   FB_PAR       curl 回退模式的并发连接数（默认 20）
#   DO_STRACE    为 1 时采集 strace -c 系统调用统计（默认 0）
#   DO_PERF      为 1 时采集 perf 热点（默认 0）
# =============================================================================
set -u

# ---------- 路径解析：脚本在 bench/ 下，项目根为其父目录 ----------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
CONF="$ROOT_DIR/conf/server.conf"

# ---------- 参数默认值 ----------
STRATEGIES="${STRATEGIES:-readwrite mmap}"
URI="${URI:-/index.html}"
CONNECTIONS="${CONNECTIONS:-100}"
DURATION="${DURATION:-10}"
REQUESTS="${REQUESTS:-20000}"
THREADS="${THREADS:-4}"
FB_PAR="${FB_PAR:-20}"
DO_STRACE="${DO_STRACE:-0}"
DO_PERF="${DO_PERF:-0}"

# ---------- 从配置读取 host/port（保证与实际监听一致） ----------
HOST="127.0.0.1"
PORT="$(grep -E '^[[:space:]]*port[[:space:]]*=' "$CONF" 2>/dev/null | head -1 | sed 's/.*=[[:space:]]*//;s/[[:space:]]*$//')"
PORT="${PORT:-8080}"
URL="http://$HOST:$PORT$URI"

# ---------- 定位/构建可执行文件 ----------
resolve_bin() {
    if [ -n "${BIN:-}" ]; then echo "$BIN"; return; fi
    for cand in "$ROOT_DIR/build/http-server" "$ROOT_DIR/out/http-server"; do
        [ -x "$cand" ] && { echo "$cand"; return; }
    done
    echo ""
}

BIN_PATH="$(resolve_bin)"
if [ -z "$BIN_PATH" ]; then
    echo "[*] 未找到 http-server 可执行文件，尝试用 cmake 构建到 build/ ..."
    if cmake -S "$ROOT_DIR" -B "$ROOT_DIR/build" >/dev/null 2>&1 \
       && cmake --build "$ROOT_DIR/build" -j"$(nproc 2>/dev/null || echo 2)" >/dev/null 2>&1; then
        BIN_PATH="$ROOT_DIR/build/http-server"
    fi
fi
if [ -z "$BIN_PATH" ] || [ ! -x "$BIN_PATH" ]; then
    echo "[!] 无法定位或构建 http-server，请先手动编译，或用 BIN=/path/to/http-server 指定。" >&2
    exit 1
fi

# ---------- 选择压测工具 ----------
# 探测所有可用工具（保持优先级顺序，供非交互回退与菜单展示）
ALL_TOOLS=()
command -v wrk       >/dev/null 2>&1 && ALL_TOOLS+=(wrk)
command -v ab        >/dev/null 2>&1 && ALL_TOOLS+=(ab)
command -v webbench  >/dev/null 2>&1 && ALL_TOOLS+=(webbench)
command -v Webbench  >/dev/null 2>&1 && ALL_TOOLS+=(Webbench)
command -v curl      >/dev/null 2>&1 && ALL_TOOLS+=(curl)   # 回退：curl keep-alive 吞吐估算

if [ "${#ALL_TOOLS[@]}" -eq 0 ]; then
    echo "[!] 未找到 wrk/ab/webbench/curl 任一压测工具，无法继续。" >&2
    exit 1
fi

# 工具选择优先级：
#   1) 显式指定 TOOL=xxx —— 必须在已探测到的可用列表里，否则报错退出
#   2) 交互式终端(stdin 是 tty)且可用工具 >1 —— 弹菜单让用户选(序号或工具名)
#   3) 其余情况(非交互/只有一个) —— 按优先级取第一个，保证可自动化
REQ_TOOL="${TOOL:-}"
if [ -n "$REQ_TOOL" ]; then
    found=""
    for t in "${ALL_TOOLS[@]}"; do [ "$t" = "$REQ_TOOL" ] && { found="$t"; break; }; done
    if [ -z "$found" ]; then
        echo "[!] 指定的 TOOL='$REQ_TOOL' 不可用；本机已探测到：${ALL_TOOLS[*]}" >&2
        exit 1
    fi
    TOOL="$found"
elif [ -t 0 ] && [ "${#ALL_TOOLS[@]}" -gt 1 ]; then
    echo "检测到多个可用压测工具，请选择本次使用哪个：" >&2
    for ((i=0;i<${#ALL_TOOLS[@]};i++)); do
        echo "  $((i+1))) ${ALL_TOOLS[$i]}" >&2
    done
    printf '输入序号或工具名 [回车默认 1 = %s]: ' "${ALL_TOOLS[0]}" >&2
    read -r choice
    if [ -z "$choice" ]; then
        TOOL="${ALL_TOOLS[0]}"
    elif [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge 1 ] && [ "$choice" -le "${#ALL_TOOLS[@]}" ]; then
        TOOL="${ALL_TOOLS[$((choice-1))]}"
    else
        matched=""
        for t in "${ALL_TOOLS[@]}"; do [ "$t" = "$choice" ] && { matched="$t"; break; }; done
        if [ -n "$matched" ]; then TOOL="$matched"; else
            echo "[!] 无效选择 '$choice'，回退到默认 ${ALL_TOOLS[0]}" >&2
            TOOL="${ALL_TOOLS[0]}"
        fi
    fi
else
    TOOL="${ALL_TOOLS[0]}"
fi

# ---------- 全局状态与清理 ----------
SRV_PID=""
ORIG_STRATEGY="$(grep -E '^[[:space:]]*static_file_strategy[[:space:]]*=' "$CONF" | head -1 | sed 's/.*=[[:space:]]*//;s/[[:space:]]*$//')"

cleanup() {
    stop_server
    # 还原配置里原来的策略，避免污染
    if [ -n "$ORIG_STRATEGY" ]; then
        sed -i "s/^static_file_strategy[[:space:]]*=.*/static_file_strategy = $ORIG_STRATEGY/" "$CONF"
    fi
}
trap cleanup EXIT INT TERM

set_strategy() {
    sed -i "s/^static_file_strategy[[:space:]]*=.*/static_file_strategy = $1/" "$CONF"
}

wait_port_free() {
    # 端口不再接受连接即视为已释放（最多等 ~5s）
    for _ in $(seq 1 50); do
        curl -s -o /dev/null --max-time 1 "$URL" 2>/dev/null || return 0
        sleep 0.1
    done
}

start_server() {
    wait_port_free
    # 用子 shell + exec 启动：子 shell 被替换为服务器进程，SRV_PID 即其真实 PID，
    # 且为本脚本的直接子进程，stop_server 里可可靠地 wait / 轮询。
    ( cd "$ROOT_DIR" && exec "$BIN_PATH" ) >"$ROOT_DIR/bench/.server.out" 2>&1 &
    SRV_PID=$!
    # 等待就绪；同时确认进程没有因 bind 失败等提前退出（否则会误测到别的残留实例）
    for _ in $(seq 1 50); do
        if ! kill -0 "$SRV_PID" 2>/dev/null; then
            echo "[!] 服务器进程启动后立即退出，bench/.server.out 内容：" >&2
            sed 's/^/    /' "$ROOT_DIR/bench/.server.out" >&2
            SRV_PID=""
            return 1
        fi
        if curl -s -o /dev/null --max-time 1 "$URL"; then return 0; fi
        sleep 0.1
    done
    echo "[!] 服务器未在预期时间内就绪，请检查 bench/.server.out" >&2
    return 1
}

stop_server() {
    [ -n "$SRV_PID" ] || return 0
    kill -TERM "$SRV_PID" 2>/dev/null
    # 轮询等待进程真正退出（优雅退出需要一点时间），最多 ~5s，超时则 SIGKILL 兜底
    for _ in $(seq 1 50); do
        kill -0 "$SRV_PID" 2>/dev/null || break
        sleep 0.1
    done
    if kill -0 "$SRV_PID" 2>/dev/null; then
        echo "[!] 服务器未在 5s 内优雅退出，强制 SIGKILL" >&2
        kill -KILL "$SRV_PID" 2>/dev/null
        sleep 0.2
    fi
    wait "$SRV_PID" 2>/dev/null
    SRV_PID=""
    wait_port_free
}

# ---------- 各工具的 QPS 采集（结果写入全局 QPS 变量） ----------
QPS=""
bench_wrk() {
    local out; out="$(wrk -t"$THREADS" -c"$CONNECTIONS" -d"${DURATION}s" "$URL" 2>&1)"
    echo "$out"
    QPS="$(echo "$out" | awk '/Requests\/sec:/{print $2}')"
}
bench_ab() {
    local out; out="$(ab -n "$REQUESTS" -c "$CONNECTIONS" "$URL" 2>&1)"
    echo "$out" | grep -E 'Requests per second|Failed requests|Time per request' 
    QPS="$(echo "$out" | awk '/Requests per second:/{print $4}')"
}
bench_webbench() {
    local bin=webbench; command -v webbench >/dev/null 2>&1 || bin=Webbench
    local out; out="$($bin -c "$CONNECTIONS" -t "$DURATION" "$URL" 2>&1)"
    echo "$out"
    # webbench 输出 Speed=NNN pages/min，换算为每秒
    QPS="$(echo "$out" | awk '/Speed=/{print $2/60}')"
}
bench_curl() {
    # 回退方案：FB_PAR 条并发 keep-alive 连接，每条串行请求 REQUESTS/FB_PAR 次，
    # 用总请求数 / 墙钟时间估算聚合 QPS（相对值足够做策略对比）。
    local per=$(( REQUESTS / FB_PAR )); [ "$per" -lt 1 ] && per=1
    # 同一条 curl 传多个 URL 会复用 keep-alive 连接；注意 -o 只对首个 URL 生效，
    # 故整体把 stdout 重定向到 /dev/null，避免其余响应体刷屏。
    local args=(); for ((i=0;i<per;i++)); do args+=("$URL"); done
    local start end elapsed_ms
    start="$(date +%s%N)"
    local pids=()
    for ((w=0;w<FB_PAR;w++)); do
        curl -s "${args[@]}" >/dev/null 2>&1 &
        pids+=($!)
    done
    # 必须显式只 wait 这些 curl 的 PID：服务器也是本脚本的后台子进程，
    # 裸 wait 会连服务器一起等，导致永久阻塞。
    wait "${pids[@]}"
    end="$(date +%s%N)"
    elapsed_ms=$(( (end - start) / 1000000 )); [ "$elapsed_ms" -lt 1 ] && elapsed_ms=1
    local ok=$(( FB_PAR * per ))
    echo "curl fallback: $ok reqs / ${elapsed_ms}ms (par=$FB_PAR)"
    QPS="$(awk "BEGIN{printf \"%.2f\", $ok/($elapsed_ms/1000)}")"
}

run_bench() {
    QPS=""
    case "$TOOL" in
        wrk)          bench_wrk ;;
        ab)           bench_ab ;;
        webbench|Webbench) bench_webbench ;;
        curl)         bench_curl ;;
    esac
    [ -z "$QPS" ] && QPS="N/A"
}

# 可选：采集系统调用统计（需 strace，且需要权限）
collect_strace() {
    command -v strace >/dev/null 2>&1 || { echo "(strace 未安装，跳过)"; return; }
    [ -n "$SRV_PID" ] || return
    local t=$(( DURATION / 2 )); [ "$t" -lt 3 ] && t=3
    # 在后台压测的同时 attach strace 统计系统调用
    ( run_bench >/dev/null 2>&1 ) &
    local bp=$!   # 压测任务子 shell 的 PID（区别于长驻的 SRV_PID）
    strace -c -f -p "$SRV_PID" 2>"$ROOT_DIR/bench/.strace.$1.txt" &
    local sp=$!
    sleep "$t"; kill -INT "$sp" 2>/dev/null; wait "$sp" 2>/dev/null
    # 只等压测任务，绝不能裸 wait —— 否则会把长驻的 http-server(SRV_PID) 也当等待目标而永久阻塞
    wait "$bp" 2>/dev/null
    echo "--- strace -c ($1) 摘要 ---"
    local sf="$ROOT_DIR/bench/.strace.$1.txt"
    if grep -qiE 'not permitted|ptrace' "$sf" 2>/dev/null; then
        # ptrace 权限受限导致 attach 失败：明确提示，不要把静默空表误当成成功
        echo "[!] strace 无法 attach（ptrace 权限受限）。本机 ptrace_scope=$(cat /proc/sys/kernel/yama/ptrace_scope 2>/dev/null)。" >&2
        echo "    修法：sudo sysctl -w kernel.yama.ptrace_scope=0  或  以 sudo 运行本脚本。原始错误：" >&2
        grep -i 'permitted' "$sf" | head -1 | sed 's/^/    /' >&2
    elif ! grep -qE '% time|total time' "$sf" 2>/dev/null; then
        echo "[!] strace 未产出统计表（可能无流量或提前退出），尾部输出：" >&2
        tail -3 "$sf" 2>/dev/null | sed 's/^/    /' >&2
    else
        grep -E 'read|write|sendfile|mmap|readv|writev|% time' "$sf" 2>/dev/null | head -20
    fi
}

collect_perf() {
    command -v perf >/dev/null 2>&1 || { echo "(perf 未安装，跳过)"; return; }
    [ -n "$SRV_PID" ] || return
    local t=$(( DURATION / 2 )); [ "$t" -lt 3 ] && t=3
    local perfdata="$ROOT_DIR/bench/.perf.$1.data"
    # 持续施压直到采集窗口结束：否则 ab 20000 请求 <1s 跑完，采样几乎全落在空闲 epoll_wait，热点失真
    local deadline=$(( $(date +%s) + t + 1 ))
    ( while [ "$(date +%s)" -lt "$deadline" ]; do run_bench >/dev/null 2>&1 || true; done ) &
    local bp=$!   # 压测循环子 shell 的 PID（区别于长驻的 SRV_PID）
    perf record -p "$SRV_PID" -g -o "$perfdata" -- sleep "$t" >/dev/null 2>&1
    # 只等压测任务，绝不能裸 wait —— 否则会把长驻的 http-server 也当等待目标而永久阻塞
    wait "$bp" 2>/dev/null

    # 用 flat self% 视图（--no-children -g none）：才能看到真正的叶子热点函数，而非线程入口调用树根帧
    local report
    report="$(perf report -i "$perfdata" --stdio --no-children -g none --percent-limit 0.3 2>/dev/null | grep -vE '^#')"
    echo "--- perf report ($1) Top 叶子热点 (self%) ---"
    if [ -z "$report" ]; then
        echo "[!] perf 无数据（可能 perf_event_paranoid=$(cat /proc/sys/kernel/perf_event_paranoid 2>/dev/null) 权限受限或无流量）。修法：sudo sysctl -w kernel.perf_event_paranoid=-1 或以 sudo 运行。" >&2
        return
    fi
    echo "$report" | head -18
    echo "--- 策略相关关键符号（文件发送/拷贝/缺页/映射路径）---"
    perf report -i "$perfdata" --stdio --no-children -g none 2>/dev/null \
        | grep -iE 'mmap|munmap|page.?fault|copy.?user|memcpy|memmove|__write|__read|sendfile|tcp_send|StaticFile|sendWith' \
        | head -12
}

# =============================================================================
# 主流程
# =============================================================================
echo "============================================================"
echo " HTTP Server 8.6 压测对比"
echo "   binary      : $BIN_PATH"
echo "   tool        : $TOOL"
echo "   url         : $URL"
echo "   connections : $CONNECTIONS   duration: ${DURATION}s   requests: $REQUESTS"
echo "   strategies  : $STRATEGIES"
echo "============================================================"

declare -A RESULT
for strat in $STRATEGIES; do
    echo
    echo "################ strategy = $strat ################"
    set_strategy "$strat"
    if ! start_server; then
        RESULT[$strat]="启动失败"
        stop_server
        continue
    fi

    run_bench
    # 压测后确认服务器仍存活：若中途崩溃/退出，本轮 QPS 无效（避免“崩了还照报数据”）
    if ! kill -0 "$SRV_PID" 2>/dev/null; then
        echo "[!] 压测期间服务器进程已退出/崩溃！本轮 QPS 无效。bench/.server.out 尾部：" >&2
        tail -5 "$ROOT_DIR/bench/.server.out" 2>/dev/null | sed 's/^/    /' >&2
        RESULT[$strat]="CRASHED"
        stop_server
        continue
    fi
    RESULT[$strat]="$QPS"
    echo ">>> [$strat] QPS = $QPS"

    if [ "$DO_STRACE" = "1" ]; then collect_strace "$strat"; fi
    if [ "$DO_PERF" = "1" ]; then collect_perf "$strat"; fi

    stop_server
done

# ---------- 汇总对比 ----------
echo
echo "============================================================"
echo " 对比汇总 (QPS)"
echo "------------------------------------------------------------"
printf " %-14s %14s %12s\n" "strategy" "QPS" "vs baseline"
base=""
for strat in $STRATEGIES; do
    q="${RESULT[$strat]:-N/A}"
    if [ -z "$base" ]; then base="$q"; bnote="(baseline)"; else
        bnote="$(awk "BEGIN{ if (\"$base\"+0>0 && \"$q\"+0>0) printf \"%+.1f%%\", (\"$q\"-\"$base\")/\"$base\"*100; else print \"-\" }")"
    fi
    printf " %-14s %14s %12s\n" "$strat" "$q" "$bnote"
done
echo "============================================================"
echo "提示：把上表 QPS 与 (可选的) strace/perf 结果记录进 README 的「压测结果」与 docs/pitfalls.md。"
