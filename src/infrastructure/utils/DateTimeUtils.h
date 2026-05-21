#pragma once
#include <string>

namespace Infrastructure {

class DateTimeUtils {
public:
    // Retorna fecha actual como "YYYY-MM-DD"
    [[nodiscard]] static std::string currentDate();

    // Retorna timestamp actual como "YYYY-MM-DD HH:MM:SS"
    [[nodiscard]] static std::string currentTimestamp();
};

} // namespace Infrastructure