#include "Matrix.h"
#include <iostream>
#include <iomanip>

int main()
{
    Matrix m(3, 3);
    Matrix n(3, 3);
    Matrix e(3, 3);


    //double val = -1e-99;
    //for (size_t i = 0; i < m.rows(); i++)
    //{
    //    for (size_t j = 0; j < m.cols(); j++)
    //    {
    //        m.set(i, j, val += 1e-100);
    //    }
    //}
    //std::cout << std::setw(10) << 3.14f << std::endl;


    for (size_t i = 0; i < m.rows(); i++)
    {
        m.set(i, i, 2);
    }

    int val = 0;
    for (size_t i = 0; i < n.rows(); i++)
    {
        for (size_t j = 0; j < n.cols(); j++)
        {
            n.set(i, j, ++val);
        }
    }

    std::cout << m << std::endl;
    std::cout << n << std::endl;
    std::cout << e << std::endl;

    e = m;

    e = e - e;

    std::cout << m << std::endl;
    std::cout << n << std::endl;
    std::cout << e << std::endl;

    return 0;
}
