#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <iomanip>

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

    // for formatted output, decides wether signs are necessary
    // amount of negative numbers
    int _negatives;

public:
    Mat() = delete;

    // initializes matrix with rows and cols and value 0
    Mat(size_t rows, size_t cols, T const& initVal = T(0))
        : _m(rows, std::vector<T>(cols, initVal))
        , _rows(rows)
        , _cols(cols)
        , _negatives(0)
    { }

    void set(size_t row, size_t col, T const& val)
    {
        if (row < _rows && col < _cols)
        {
            if (_m[row][col] < 0 && val >= 0)
            {
                --_negatives;
            }
            else if (_m[row][col] >= 0 && val < 0)
            {
                ++_negatives;
            }

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

    Mat const& operator-= (Mat const& right)
    {
        return *this = *this - right;
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
        if (m._negatives > 0)
        {
            ostr << std::showpos;
        }

        for (size_t i = 0; i < m._rows; i++)
        {
            for (size_t j = 0; j < m._cols; j++)
            {
                int precision = 2;
                int width = precision +6;

                // <= -1e+100
                // <= -1e+10
                // <= -1e-100
                // <= +1e-10
                // <= +1e+100


                //TODO foramtting of (absolute) larger and very small numbers
                if (   m._m[i][j] < -1e+100
                    || m._m[i][j] > +1e+100
                    || (   m._m[i][j] > -1e-99
                        && m._m[i][j] < +1e-100)
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
