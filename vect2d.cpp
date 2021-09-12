#include "vect2d.h"

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
