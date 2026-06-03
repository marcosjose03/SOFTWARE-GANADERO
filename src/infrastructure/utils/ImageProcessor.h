#pragma once
#include <vector>
#include <cstdint>
#include <optional>
#include <string>

namespace Infrastructure {

class ImageProcessor {
public:
    // Recibe bytes de cualquier imagen, devuelve PNG 255x255
    [[nodiscard]] static std::optional<std::vector<uint8_t>>
    resizeToThumbnail(const std::vector<uint8_t>& inputBytes);
};

} // namespace Infrastructure