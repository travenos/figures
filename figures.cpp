#include "figures.h"
#include "canvas.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>

//=========================================================================

Vect2D Vect2D::operator+() const noexcept
{
    return *this;
}

Vect2D Vect2D::operator-() const noexcept
{
    return Vect2D{-x, -y};
}

Vect2D operator+(const Vect2D& vect1, const Vect2D& vect2) noexcept
{
    return Vect2D{vect1.x + vect2.x, vect1.y + vect2.y};
}

Vect2D operator-(const Vect2D& vect1, const Vect2D& vect2) noexcept
{
    return vect1 + (-vect2);
}

Vect2D& Vect2D::operator+=(const Vect2D& other) noexcept
{
    *this = *this + other;
    return *this;
}

Vect2D& Vect2D::operator-=(const Vect2D& other) noexcept
{
    *this = *this - other;
    return *this;
}

//=========================================================================

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
    const int x1 = getTopLeft().x;
    const int x2 = getBottomRight().x;
    const int y1 = getTopLeft().y;
    const int y2 = getBottomRight().y;

    const int minX = std::max(getTopLeft().x, 0);
    const int maxX = std::min(getBottomRight().x, canvas.getWidth() - 1);
    const int minY = std::max(getTopLeft().y, 0);
    const int maxY = std::min(getBottomRight().y, canvas.getHeight() - 1);

    for (int x{minX}; x <= maxX; ++x)
    {
        if (y1 >= 0 && y1 < canvas.getHeight())
        {
            canvas.at(x, y1) = getColor();
        }
        if (y2 >= 0 && y2 < canvas.getHeight())
        {
            canvas.at(x, y2) = getColor();
        }
    }

    for (int y{minY}; y <= maxY; ++y)
    {
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
    struct Side
    {
        double k;
        double b;
        int min;
        int max;
        bool yFromX;
    };

    const std::pair<const Vect2D&, const Vect2D&> nodePairs[] = {{m_pt1, m_pt2}, {m_pt1, m_pt3}, {m_pt2, m_pt3}};
    constexpr std::size_t PairsCount = sizeof(nodePairs) / sizeof(*nodePairs);
    Side sides[PairsCount];

    for (std::size_t i{0}; i < PairsCount; ++i)
    {
        const int x1 = nodePairs[i].first.x;
        const int x2 = nodePairs[i].second.x;
        const int y1 = nodePairs[i].first.y;
        const int y2 = nodePairs[i].second.y;
        sides[i].yFromX = (std::abs(x1 - x2) >= std::abs(y1 - y2));
        if (sides[i].yFromX)
        {
            sides[i].k = x1 != x2 ? static_cast<double>(y1 - y2) / (x1 - x2) : 0;
            sides[i].b = y1 - sides[i].k * x1;
            sides[i].min = std::min(x1, x2);
            sides[i].max = std::max(x1, x2);
        }
        else
        {
            sides[i].k = y1 != y2 ? static_cast<double>(x1 - x2) / (y1 - y2) : 0;
            sides[i].b = x1 - sides[i].k * y1;
            sides[i].min = std::min(y1, y2);
            sides[i].max = std::max(y1, y2);
        }
    }

    for (const auto& side: sides)
    {
        if (side.yFromX)
        {
            const int minX = std::max(side.min, 0);
            const int maxX = std::min(side.max, canvas.getWidth() - 1);
            for (int x{minX}; x <= maxX; ++x)
            {
                const int y = static_cast<int>(std::round(side.k * x + side.b));
                if (y >= 0 && y < canvas.getHeight())
                {
                    canvas.at(x, y) = getColor();
                }
            }
        }
        else
        {
            const int minY = std::max(side.min, 0);
            const int maxY = std::min(side.max, canvas.getHeight() - 1);
            for (int y{minY}; y <= maxY; ++y)
            {
                const int x = static_cast<int>(std::round(side.k * y + side.b));
                if (x >= 0 && x < canvas.getWidth())
                {
                    canvas.at(x, y) = getColor();
                }
            }
        }
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
