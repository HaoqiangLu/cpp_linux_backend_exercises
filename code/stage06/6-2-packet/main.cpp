#include "PacketCodec.h"
#include <iostream>

int main() {
    auto cb = [](const std::string& m) {
        std::cout << "[msg] " << m << std::endl;
    };

    std::cout << "===== FixedLengthCodec =====" << std::endl;
    FixedLengthCodec fc(8);
    const char* d1 = "helloXXXworldXXX";    // 两条定长消息
    fc.onData(d1, 16, cb);

    std::cout << "===== DelimiterCodec =====" << std::endl;
    DelimiterCodec dc;
    dc.onData("hello\nwor", 8, cb); // 半包
    dc.onData("ld\nfoo\n", 8, cb);  // 粘包 + 补齐

    std::cout << "===== LengthFieldCodec =====" << std::endl;
    auto appendFrame = [](std::string& buf, const std::string& body) {
        std::uint32_t n = static_cast<std::uint32_t>(body.size());
        buf.append(reinterpret_cast<const char*>(&n), sizeof(n));
        buf.append(body);
    };
    std::string raw;
    appendFrame(raw, "hello");
    appendFrame(raw, "world!!");
    // raw 共 20 字节

    // 场景A: 一次性喂入 → 粘包，拆除两条完整消息
    std::cout << "===== LengthFieldCodec - Scene A =====" << std::endl;
    LengthFieldCodec lc;
    lc.onData(raw.data(), raw.size(), cb);
    // 期望输出：[msg] hello / [msg] world!!

    // 场景B: 分多次喂第一帧 → 演示半包（长度字段没读全 / body 不够，都不回调）
    std::cout << "===== LengthFieldCodec - Scene B =====" << std::endl;
    LengthFieldCodec lc2;
    lc2.onData(raw.data(),     3, cb);  // buffer=3 <4, 不够回调
    lc2.onData(raw.data() + 3, 5, cb);  // buffer=8, 长度=5 但 body 只到 8<4+5, break, 不回调
    lc2.onData(raw.data() + 8, 1, cb);  // buffer=9, 凑齐帧1, 回调 "hello"，剩 8
    // 此时 raw 还剩 11 字节未喂，可继续 appendFrame 后续喂入还原 "world!!"
    lc2.onData(raw.data() + 9, raw.size() - 9, cb); // 把 raw 剩余的 11 字节(帧2)喂完
}