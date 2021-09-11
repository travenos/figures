#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

struct ColorBgr final
{
    std::uint8_t B{255};
    std::uint8_t G{255};
    std::uint8_t R{255};
};

#endif // COLOR_H
