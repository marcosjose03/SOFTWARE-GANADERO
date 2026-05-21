#pragma once
#include <string>

namespace Infrastructure {

class UuidGenerator {
public:
    [[nodiscard]] static std::string generate();
};

} // namespace Infrastructure