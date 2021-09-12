#include "figures.h"
#include "canvas.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>

Figure::~Figure() noexcept = default;

Figure::Figure(const Vect2D& topLeft, const Vect2D& bottomRight) noexcept :
    m_topLeft{topLeft}
  , m_bottomRight{bottomRight}
{
}

const Vect2D& Figure::getTopLeft() const noexcept
{
    return m_topLeft;
}

const Vect2D& Figure::getBottomRight() const noexcept
{
    return m_bottomRight;
}

const ColorBgr& Figure::getColor() const noexcept
{
    return m_color;
}

void Figure::setColor(const ColorBgr& color) noexcept
{
    m_color = color;
}

//=========================================================================

Rectangle::Rectangle(const Vect2D& topLeft, const Vect2D& bottomRight) noexcept :
    Figure{topLeft, bottomRight}
{
}

void Rectangle::draw(Canvas& canvas) const noexcept
{
    const Vect2D& topLeft{getTopLeft()};
    const Vect2D& bottomRight{getBottomRight()};

    const Vect2D rectangleSize = bottomRight - topLeft;

    const Vect2D topRight{topLeft.x + rectangleSize.x, topLeft.y};
    const Vect2D bottomLeft{topLeft.x, topLeft.y + rectangleSize.y};

    canvas.drawLine(topLeft, topRight, getColor());
    canvas.drawLine(topRight, bottomRight, getColor());
    canvas.drawLine(bottomRight, bottomLeft, getColor());
    canvas.drawLine(bottomLeft, topLeft, getColor());
}

//=========================================================================

Square::Square(const Vect2D& topLeft, const int side) noexcept :
    Rectangle{topLeft, Vect2D{topLeft.x + std::abs(side), topLeft.y + std::abs(side)}}
{
}

//=========================================================================

Triangle::Triangle(const Vect2D& pt1, const Vect2D& pt2, const Vect2D& pt3) noexcept :
    Figure{
        Vect2D{std::min(std::min(pt1.x, pt2.x), pt3.x), std::min(std::min(pt1.y, pt2.y), pt3.y)},
        Vect2D{std::max(std::min(pt1.x, pt2.x), pt3.x), std::max(std::min(pt1.y, pt2.y), pt3.y)}
        }
  , m_pt1{pt1}
  , m_pt2{pt2}
  , m_pt3{pt3}
{
}

void Triangle::draw(Canvas& canvas) const noexcept
{
    const std::pair<const Vect2D&, const Vect2D&> nodePairs[] = {{m_pt1, m_pt2}, {m_pt1, m_pt3}, {m_pt2, m_pt3}};
    for (const auto& pair: nodePairs)
    {
        canvas.drawLine(pair.first, pair.second, getColor());
    }
}

//=========================================================================

Circle::Circle(const Vect2D& center, const int radius) noexcept :
    Figure{
        Vect2D{center.x - std::abs(radius), center.y - std::abs(radius)},
        Vect2D{center.x + std::abs(radius), center.y + std::abs(radius)}
        }
  , m_center{center}
  , m_radius{std::abs(radius)}
{
}

void Circle::draw(Canvas& canvas) const noexcept
{
    constexpr double sin45 = 0.707106781;
    int startX = static_cast<int>(getTopLeft().x + std::ceil(m_radius * (1 - sin45)));
    startX = std::max(startX, 0);
    int endX = static_cast<int>(getBottomRight().x - std::ceil(m_radius * (1 - sin45)));
    endX = std::max(endX, canvas.getWidth() - 1);
    int startY = static_cast<int>(getTopLeft().y + std::ceil(m_radius * (1 - sin45)));
    startY = std::max(startY, 0);
    int endY = static_cast<int>(getBottomRight().y - std::ceil(m_radius * (1 - sin45)));
    endY = std::max(endY, canvas.getHeight() - 1);
    const double r2 = m_radius * m_radius;

    for (int x{startX}; x <= endX; ++x)
    {
        const int dx = x - m_center.x;
        const double yPart = std::sqrt(r2 - dx * dx);
        const int y1 = static_cast<int>(std::round(yPart + m_center.y));
        const int y2 = static_cast<int>(std::round(-yPart + m_center.y));
        if (y1 >= 0 && y1 < canvas.getHeight())
        {
            canvas.at(x, y1) = getColor();
        }
        if (y2 >= 0 && y2 < canvas.getHeight())
        {
            canvas.at(x, y2) = getColor();
        }
    }

    for (int y{startY}; y <= endY; ++y)
    {
        const int dy = y - m_center.y;
        const double xPart = std::sqrt(r2 - dy * dy);
        const int x1 = static_cast<int>(std::round(xPart + m_center.x));
        const int x2 = static_cast<int>(std::round(-xPart + m_center.x));
        if (x1 >= 0 && x1 < canvas.getWidth())
        {
            canvas.at(x1, y) = getColor();
        }
        if (x2 >= 0 && x2 < canvas.getWidth())
        {
            canvas.at(x2, y) = getColor();
        }
    }
}
