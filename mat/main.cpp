#include "Matrix.h"
#include <iostream>
#include <iomanip>

#include "olcPixelGameEngine.h"
#include "Plot.h"

int main()
{
    Matrix m(3, 2);
    Matrix b(3, 1);
    b.set(0, 0, 1);
    b.set(1, 0, 2);
    b.set(2, 0, 3);

    m.set(0, 0, 1);    m.set(0, 1, 1);
    m.set(1, 0, 1);    m.set(1, 1, 3);
    m.set(2, 0, 1);    m.set(2, 1, 4);

    std::cout << m << std::endl;
    std::cout << b << std::endl;

    Matrix erg = Matrix::num2u1a5(m, b);

    std::cout << erg << std::endl;

    Plot demo;
    float scale = 8;
    int width = 1600 / scale;
    int height = 960 / scale;
    if (demo.Construct(width, height, scale, scale))
    {
        demo.Start();
    }

    return 0;
}
