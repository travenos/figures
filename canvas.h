#ifndef CANVAS_H
#define CANVAS_H

#include "color.h"
#include "vect2d.h"

#include <vector>
#include <string>

class Canvas final
{
public:
    Canvas(int width, int height) noexcept(false);
    void saveToBmp(const std::string& filename) const noexcept(false);

    ColorBgr& at(int x, int y) noexcept(false);
    const ColorBgr& at(int x, int y) const noexcept(false);

    void drawLine(const Vect2D& pt1, const Vect2D& pt2, const ColorBgr& color = ColorBgr{0, 0, 0}) noexcept;

    int getWidth() const noexcept
    {
        return m_width;
    }

    int getHeight() const noexcept
    {
        return m_height;
    }
private:
    std::vector<ColorBgr> m_data{};
    int m_width{};
    int m_height{};
};

#endif // CANVAS_H
