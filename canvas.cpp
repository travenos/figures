#include "canvas.h"

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <fstream>
#include <stdexcept>

#pragma pack(push, 2)

struct BmpHeader
{
    char type[2];            // Magic identifier BM
    std::uint32_t fileSize;  // File size in bytes
    std::uint32_t reserved;
    std::uint32_t offset;    // Offset to image data, bytes
} ;

struct BmpInfoHeader
{
    std::uint32_t headerSize;       // Header size in bytes
    std::int32_t width;
    std::int32_t height;            // Width and height of image
    std::uint16_t planes;           // Number of colour planes
    std::uint16_t bitsPerPixel;     // Bits per pixel
    std::uint32_t compression;      // Compression type
    std::uint32_t imageSize;        // Image size in bytes
    std::int32_t xResolution;
    std::int32_t yResolution;       // Pixels per meter
    std::uint32_t nColours;         // Number of colours
    std::uint32_t importantColours; // Important colours
};

#pragma pack(pop)

Canvas::Canvas(int width, int height) noexcept(false) :
  m_width{width}
  , m_height{height}
{
    if (m_width >= 0 && m_height >= 0)
    {
        m_data.resize(static_cast<std::size_t>(m_width * m_height)); // Can throw std::bad_alloc
    }
    else
    {
        throw std::invalid_argument("Got invalid canvas size");
    }
}

void Canvas::saveToBmp(const std::string& filename) const noexcept(false)
{
    constexpr std::uint32_t bytesPerPixel = sizeof(ColorBgr);
    constexpr std::uint32_t dataOffset = sizeof(BmpHeader) + sizeof(BmpInfoHeader);
    constexpr std::int32_t resolution = 11811; // 300 DPI

    const auto paddedRowSize = static_cast<std::uint32_t>(4 * std::ceil((m_width * static_cast<double>(bytesPerPixel)) / 4.0));
    const int rowSize = m_width * static_cast<int>(bytesPerPixel);
    const int paddingSize = static_cast<int>(paddedRowSize) - rowSize;

    BmpHeader bmpHeader;
    std::memcpy(bmpHeader.type, "BM", sizeof(bmpHeader.type));
    bmpHeader.fileSize = paddedRowSize * static_cast<std::uint32_t>(m_height) + dataOffset;
    bmpHeader.reserved = 0;
    bmpHeader.offset = dataOffset;

    BmpInfoHeader bmpInfoHeader;
    bmpInfoHeader.headerSize = sizeof(bmpInfoHeader);
    bmpInfoHeader.width = m_width;
    bmpInfoHeader.height = m_height;
    bmpInfoHeader.planes = 1;
    bmpInfoHeader.bitsPerPixel = bytesPerPixel * 8;
    bmpInfoHeader.compression = 0;
    bmpInfoHeader.imageSize = static_cast<std::uint32_t>(m_width * m_height) * bytesPerPixel;
    bmpInfoHeader.xResolution = resolution;
    bmpInfoHeader.yResolution = resolution;
    bmpInfoHeader.nColours = (1 << (sizeof(ColorBgr) * 8));
    bmpInfoHeader.importantColours = 0;

    std::ofstream file{filename, std::ios::out | std::ios::binary};
    file.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader)); // Can throw std::ios_base::failure
    file.write(reinterpret_cast<const char*>(&bmpInfoHeader), sizeof(bmpInfoHeader)); // Can throw std::ios_base::failure

    const ColorBgr* imageDataPtr = m_data.data() + m_data.size() - m_width;
    for (int i{0}; i < m_height; ++i)
    {
        file.write(reinterpret_cast<const char*>(imageDataPtr), rowSize); // Can throw std::ios_base::failure
        static const char zero = 0;
        for (int j{0}; j < paddingSize; ++j)
        {
            file.write(&zero, sizeof(zero)); // Can throw std::ios_base::failure
        }
        imageDataPtr -= m_width;
    }
    if (!file.good())
    {
        throw std::ios_base::failure{"Failed to write the file " + filename};
    }
}

ColorBgr& Canvas::at(int x, int y) noexcept(false)
{
    if (x >= 0 && y >= 0)
    {
        const std::size_t pos = static_cast<std::size_t>(y * m_width + x);
        return m_data.at(pos); // Can throw std::out_of_range
    }
    else
    {
        throw std::out_of_range("At least one of the coordinates is negative");
    }
}

const ColorBgr& Canvas::at(int x, int y) const noexcept(false)
{
    if (x >= 0 && y >= 0)
    {
        const std::size_t pos = static_cast<std::size_t>(y * m_width + x);
        return m_data.at(pos); // Can throw std::out_of_range
    }
    else
    {
        throw std::out_of_range("At least one of the coordinates is negative");
    }
}
