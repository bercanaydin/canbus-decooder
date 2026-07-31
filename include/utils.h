#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <cstdint>



// Hex (Onaltılık) metni (Örn: "1A6" veya "C4") C++'ın anlayacağı sayılara çeviren yardımcı fonksiyon
inline uint32_t hexToUInt(const std::string& hexStr) {
    uint32_t val;
    std::stringstream ss;
    ss << std::hex << hexStr;
    ss >> val;
    return val;
}

#endif