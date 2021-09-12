#ifndef VECT2D_H
#define VECT2D_H

struct Vect2D final
{
    int x{};
    int y{};

    Vect2D operator+() const noexcept;
    Vect2D operator-() const noexcept;
    Vect2D& operator+=(const Vect2D& other) noexcept;
    Vect2D& operator-=(const Vect2D& other) noexcept;
};

Vect2D operator+(const Vect2D& vect1, const Vect2D& vect2) noexcept;
Vect2D operator-(const Vect2D& vect1, const Vect2D& vect2) noexcept;

#endif // VECT2D_H
