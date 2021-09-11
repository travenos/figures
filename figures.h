#ifndef FIGURES_H
#define FIGURES_H

#include "color.h"

class Canvas;

//=========================================================================

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


//=========================================================================

class Figure
{
public:
    Figure() noexcept = default;
    virtual ~Figure() noexcept;
    Figure(const Vect2D& topLeft, const Vect2D& bottomRight) noexcept;
    const Vect2D& getTopLeft() const noexcept;
    const Vect2D& getBottomRight() const noexcept;
    const ColorBgr& getColor() const noexcept;
    void setColor(const ColorBgr& color) noexcept;

    virtual void draw(Canvas& canvas) const noexcept = 0;

private:
    Vect2D m_topLeft{};
    Vect2D m_bottomRight{};
    ColorBgr m_color{0, 0 ,0};
};

//=========================================================================

class Rectangle: public Figure
{
public:
    Rectangle() noexcept = default;
    Rectangle(const Vect2D& topLeft, const Vect2D& bottomRight) noexcept;
    void draw(Canvas& canvas) const noexcept override;
};

//=========================================================================

class Square final: public Rectangle
{
public:
    Square() noexcept = default;
    Square(const Vect2D& topLeft, const int side) noexcept;
};

//=========================================================================

class Triangle final: public Figure
{
public:
    Triangle() noexcept = default;
    Triangle(const Vect2D& pt1, const Vect2D& pt2, const Vect2D& pt3) noexcept;

    void draw(Canvas& canvas) const noexcept override;

private:
    Vect2D m_pt1{};
    Vect2D m_pt2{};
    Vect2D m_pt3{};
};

//=========================================================================

class Circle final: public Figure
{
public:
    Circle() noexcept = default;
    Circle(const Vect2D& center, const int radius) noexcept;

    void draw(Canvas& canvas) const noexcept override;

private:
    Vect2D m_center{};
    int m_radius{};
};

#endif // FIGURES_H
