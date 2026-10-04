#include "PacketCodec.h"
#include <arpa/inet.h>
#include <cstdint>

// 练习1
FixedLengthCodec::FixedLengthCodec(size_t fixedLen) : fixedLen_(fixedLen) {

}

void FixedLengthCodec::onData(const char* data, size_t len, const MessageCallback& cb) {
    buffer_.append(data, len);
    while (buffer_.size() >= fixedLen_) {
        cb(buffer_.substr(0, fixedLen_));
        buffer_.erase(0, fixedLen_);
    }
}


// 练习2
DelimiterCodec::DelimiterCodec(char delim) : delim_(delim) {

}

void DelimiterCodec::onData(const char* data, size_t len, const MessageCallback& cb) {
    buffer_.append(data, len);
    auto pos = buffer_.find(delim_);
    while (pos != std::string::npos) {
        cb(buffer_.substr(0, pos));
        buffer_.erase(0, pos + 1);
        pos = buffer_.find(delim_);
    }
}

// 练习3
void LengthFieldCodec::onData(const char* data, size_t len, const MessageCallback& cb) {
    buffer_.append(data, len);
    while (buffer_.size() >= 4) {
        std::uint32_t bodyLen = *reinterpret_cast<const std::uint32_t*>(buffer_.data());
        if (buffer_.size() < 4 + bodyLen) break;    // 半包

        cb(buffer_.substr(4, bodyLen));
        buffer_.erase(0, 4 + bodyLen);
    }
}