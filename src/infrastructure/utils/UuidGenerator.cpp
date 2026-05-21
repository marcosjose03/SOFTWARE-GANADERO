#include "UuidGenerator.h"
#include <random>
#include <sstream>
#include <iomanip>

namespace Infrastructure {

std::string UuidGenerator::generate() {
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::ostringstream ss;
    for (int i = 0; i < 8;  i++) ss << std::hex << dis(gen);
    ss << "-";
    for (int i = 0; i < 4;  i++) ss << std::hex << dis(gen);
    ss << "-4";
    for (int i = 0; i < 3;  i++) ss << std::hex << dis(gen);
    ss << "-";
    ss << std::hex << dis2(gen);
    for (int i = 0; i < 3;  i++) ss << std::hex << dis(gen);
    ss << "-";
    for (int i = 0; i < 12; i++) ss << std::hex << dis(gen);

    return ss.str();
}

} // namespace Infrastructure