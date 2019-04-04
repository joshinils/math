#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm> // std::swap

// forward declare
template <typename T>
class Mat;

// typedef usual double case
typedef Mat<double> Matrix;


template <typename T>
class Mat
{
    // member that stores elements
    std::vector<std::vector<T>> _m;

    size_t _rows;
    size_t _cols;

public:
    Mat() = delete;

    // initializes matrix with rows and cols and value 0
    Mat(size_t rows, size_t cols, T const& initVal = T(0))
        : _m(rows, std::vector<T>(cols, initVal))
        , _rows(rows)
        , _cols(cols)
    { }

    void set(size_t row, size_t col, T const& val)
    {
        if (row < _rows && col < _cols)
        {
            _m[row][col] = val;
        }
        else
        {
            std::cout << "index out of bounds: " << row << " " << col << std::endl;
        }
    }

    size_t rows() const
    {
        return _rows;
    }

    size_t cols() const
    {
        return _cols;
    }

    Mat operator* (Mat const& right) const
    {
        if (this->cols() != right.rows())
        {
            throw std::invalid_argument("Mat::operator*:: left.cols != right.rows !");
        }

        Mat res(this->rows(), right.cols());

        for (size_t r = 0; r < res.rows(); r++)
        {
            for (size_t c = 0; c < res.cols(); c++)
            {
                for (size_t i = 0; i < this->cols(); i++)
                {
                    res._m[r][c] += this->_m.operator[](r)[i] * right._m[i][c];
                }
            }
        }

        return res;
    }

    Mat const& operator*= (Mat const& right)
    {
        return *this = *this * right;
    }

    Mat operator+ (Mat const& right) const
    {
        if (this->cols() != right.cols())
        {
            throw std::invalid_argument("Mat::operator+:: left.cols != right.cols !");
        }

        if (this->rows() != right.rows())
        {
            throw std::invalid_argument("Mat::operator+:: left.rows != right.rows !");
        }

        Mat res(*this);

        for (size_t r = 0; r < res.rows(); r++)
        {
            for (size_t c = 0; c < res.cols(); c++)
            {
                res._m[r][c] += right._m[r][c];
            }
        }

        return res;
    }

    Mat const& operator+= (Mat const& right)
    {
        return *this = *this + right;
    }


    Mat operator- (Mat const& right) const
    {
        if (this->cols() != right.cols())
        {
            throw std::invalid_argument("Mat::operator-:: left.cols != right.cols !");
        }

        if (this->rows() != right.rows())
        {
            throw std::invalid_argument("Mat::operator-:: left.rows != right.rows !");
        }

        Mat res(*this);

        for (size_t r = 0; r < res.rows(); r++)
        {
            for (size_t c = 0; c < res.cols(); c++)
            {
                res._m[r][c] -= right._m[r][c];
            }
        }

        return res;
    }

    void transpose()
    {
        Mat old(*this);
        std::swap(this->_rows, this->_cols);
        this->_m = std::vector<std::vector<T>>(this->rows(), std::vector<T>(this->cols())); // no initial value, gets overridden anyway

        for (size_t r = 0; r < this->rows(); r++)
        {
            for (size_t c = 0; c < this->cols(); c++)
            {
                this->_m[r][c] = old._m[c][r];
            }
        }
    }

    Mat const& operator-= (Mat const& right)
    {
        return *this = *this - right;
    }

    // Gauss-Algorithmus als LGS-Loeser (einfache Grundversion).
    // Hier wird ausnahmsweise auf Pivotisierung und das Abfangen einer
    // singulaeren Matrix verzichtet, außerdem werden die Inputdaten veraendert.
    // Diese Funktion taugt nicht zum Loesen eines LGS in einer Anwendung!
    // Sie dient allein zur Laufzeitmessung fuer den Zweck dieser Aufgabe.
    //
    // -- Danke oellrich!
    Mat lgsSolve(Mat bVektor) const
    {
        if(this->rows() != this->cols())
        {
            throw std::invalid_argument("Mat::lgsSolve not a square matrix!");
        }
        if(bVektor.cols() != 1)
        {
            throw std::invalid_argument("Mat::lgsSolve not a vector as argument!");
        }
        if(bVektor.rows() != this->rows())
        {
            throw std::invalid_argument("Mat::lgsSolve vector has wrong length!");
        }

        Mat lgsMat = *this;

        // Vorwaerts-Elimination
        for (size_t i = 0; i < lgsMat.cols(); ++i)
        {
            for (size_t k = i + 1; k < lgsMat.cols(); ++k)
            {
                T s = lgsMat._m[k][i] / lgsMat._m[i][i];

                for (size_t j = i + 1; j < lgsMat.cols(); ++j)
                {
                    lgsMat._m[k][j] -= s * lgsMat._m[i][j];
                }
                bVektor._m[k][0] -= s * bVektor._m[i][0];
            }
        }

        // Rueckwaertseinsetzen
        for (int i = (int)lgsMat.cols() - 1; i >= 0; --i)
        {
            for (size_t k = i + 1; k < lgsMat.cols(); ++k)
            {
                bVektor._m[i][0] -= lgsMat._m[i][k] * bVektor._m[k][0];
            }
            bVektor._m[i][0] /= lgsMat._m[i][i];
        }

        return bVektor;

    }  // gauss()


    static Mat num2u1a5(Mat a, Mat b)
    {
        Mat aTransposed = a;
        aTransposed.transpose();

        Mat A = aTransposed * a;

        return A.lgsSolve(aTransposed * b);
    }

    // used by formatted output
    bool hasNegatives() const
    {
        for (size_t r = 0; r < this->rows(); r++)
        {
            for (size_t c = 0; c < this->cols(); c++)
            {
                if (this->_m[r][c] < 0)
                {
                    return true;
                }
            }
        }

        return false;
    }

    // special for T == double
    // too lazy for the mumbo jumbo
    friend std::ostream& operator<< (std::ostream& ostr, Mat const& m)
    {
        // remember flags before
        //std::ios_base::fmtflags flags = ostr.flags();
        auto flags = ostr.flags();

        // output formatted
        ostr << std::right << std::scientific;
        int widthOffset = 7;
        if (m.hasNegatives())
        {
            ostr << std::showpos;
            --widthOffset;
        }

        for (size_t i = 0; i < m._rows; i++)
        {
            for (size_t j = 0; j < m._cols; j++)
            {
                int precision = 5;
                int width = precision + widthOffset;

                // <= -1e+100
                // <= -1e+10
                // <= -1e-100
                // <= +1e-10
                // <= +1e+100

                T const& val = m._m[i][j];
                long long exponent = floor(log10(abs(val)));
                if (val == 0)
                {
                    exponent = 0;
                }
                //TODO foramtting of (absolute) larger and very small numbers
                if (   val < -1e+100
                    || val > +1e+100
                    || (   val > -1e-99
                        && val < +1e-100)
                    )
                {
                    precision--;
                }
                std::cout << std::setw(width) << std::setprecision(precision) << m._m[i][j] << " ";
            }
            std::cout << std::endl;
        }

        // reset flags, no side effects
        ostr.flags(flags);

        return ostr;
    }
};

#endif // MATRIX_H
