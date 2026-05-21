#include "ImageProcessor.h"
#include <QImage>
#include <QBuffer>
#include <QByteArray>

namespace Infrastructure {

std::optional<std::vector<uint8_t>>
ImageProcessor::resizeToThumbnail(const std::vector<uint8_t>& inputBytes) {
    QByteArray raw(reinterpret_cast<const char*>(inputBytes.data()),
                   static_cast<qsizetype>(inputBytes.size()));

    QImage img;
    if (!img.loadFromData(raw))
        return std::nullopt;

    QImage scaled = img.scaled(255, 255, Qt::KeepAspectRatioByExpanding,
                                         Qt::SmoothTransformation)
                       .copy(0, 0, 255, 255);

    QByteArray out;
    QBuffer    buf(&out);
    buf.open(QIODevice::WriteOnly);
    scaled.save(&buf, "PNG");

    return std::vector<uint8_t>(out.begin(), out.end());
}

} // namespace Infrastructure