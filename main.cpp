#include "canvas.h"
#include "figures.h"

#include <iostream>
#include <exception>
#include <memory>

std::unique_ptr<Figure> enterRectangle() noexcept;
std::unique_ptr<Figure> enterSquare() noexcept;
std::unique_ptr<Figure> enterTriangle() noexcept;
std::unique_ptr<Figure> enterCircle() noexcept;
ColorBgr chooseColor() noexcept;
void saveToFile(const Canvas& canvas) noexcept;

int main()
{
    constexpr int canvasWidth{800};
    constexpr int canvasHeight{600};
    Canvas canvas{canvasWidth, canvasHeight};
    ColorBgr color{0, 0, 0}; // Black
    while (true)
    {
        std::cout << "Select an option:" << std::endl
                  << "1. Draw a rectangle" << std::endl
                  << "2. Draw a square" << std::endl
                  << "3. Draw a triangle" << std::endl
                  << "4. Draw a circle" << std::endl
                  << "5. Choose a color" << std::endl
                  << "6. Save the canvas to a BMP file" << std::endl
                  << "7. Clear the canvas" << std::endl
                  << "8. Exit" << std::endl;

        int option;
        std::unique_ptr<Figure> figure;

        std::cin >> option;
        switch (option)
        {
            case 1:
                figure = enterRectangle();
                break;
            case 2:
                figure = enterSquare();
                break;
            case 3:
                figure = enterTriangle();
                break;
            case 4:
                figure = enterCircle();
                break;
            case 5:
                color = chooseColor();
                break;
            case 6:
                saveToFile(canvas);
                break;
            case 7:
                canvas = Canvas{canvasWidth, canvasHeight};
                break;
            case 8:
                std::exit(0);
            default:
                std::cerr << "Invalid option: " << option << std::endl;
        }
        if (figure)
        {
            figure->setColor(color);
            figure->draw(canvas);
        }
    }
}

std::unique_ptr<Figure> enterRectangle() noexcept
{
    Vect2D topLeft, bottomRight;
    std::cout << "Enter left x: ";
    std::cin >> topLeft.x;
    std::cout << "Enter top y: ";
    std::cin >> topLeft.y;
    std::cout << "Enter right x: ";
    std::cin >> bottomRight.x;
    std::cout << "Enter bottom y: ";
    std::cin >> bottomRight.y;
    return std::make_unique<Rectangle>(topLeft, bottomRight);
}

std::unique_ptr<Figure> enterSquare() noexcept
{
    Vect2D topLeft;
    int side;
    std::cout << "Enter left x: ";
    std::cin >> topLeft.x;
    std::cout << "Enter top y: ";
    std::cin >> topLeft.y;
    std::cout << "Enter side length: ";
    std::cin >> side;
    return std::make_unique<Square>(topLeft, side);
}

std::unique_ptr<Figure> enterTriangle() noexcept
{
    Vect2D pt1, pt2, pt3;
    std::cout << "Enter pt1 x: ";
    std::cin >> pt1.x;
    std::cout << "Enter pt1 y: ";
    std::cin >> pt1.y;
    std::cout << "Enter pt2 x: " ;
    std::cin >> pt2.x;
    std::cout << "Enter pt2 y: ";
    std::cin >> pt2.y;
    std::cout << "Enter pt3 x: ";
    std::cin >> pt3.x;
    std::cout << "Enter pt3 y: ";
    std::cin >> pt3.y;
    return std::make_unique<Triangle>(pt1, pt2, pt3);
}

std::unique_ptr<Figure> enterCircle() noexcept
{
    Vect2D center;
    int radius;
    std::cout << "Enter center x: ";
    std::cin >> center.x;
    std::cout << "Enter center y: ";
    std::cin >> center.y;
    std::cout << "Enter radius: ";
    std::cin >> radius;
    return std::make_unique<Circle>(center, radius);
}

ColorBgr chooseColor() noexcept
{
    int red, green, blue;
    std::cout << "Enter red value: ";
    std::cin >> red;
    std::cout << "Enter green value: ";
    std::cin >> green;
    std::cout << "Enter blue value: ";
    std::cin >> blue;
    return ColorBgr{static_cast<std::uint8_t>(blue), static_cast<std::uint8_t>(green), static_cast<std::uint8_t>(red)};
}

void saveToFile(const Canvas& canvas) noexcept
try
{
    std::string filename;
    std::cout << "Enter the file name: ";
    std::cin >> filename;
    canvas.saveToBmp(filename);
}
catch (const std::exception& e)
{
    std::cerr << __PRETTY_FUNCTION__ << " Caught an exception: " << e.what() << std::endl;
}


