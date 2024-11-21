#include "Matrix.h"


inline const Matrix Matrix::operator+(const Matrix& m) const
{
    Matrix result(*this);
    result.add(m);
    return result;
}

inline Matrix& Matrix::operator+=(const Matrix& m)
{
    add(m);
    return *this;
}

inline const Matrix Matrix::operator-(const Matrix& m) const
{
    Matrix result(*this);
    result.subtract(m);
    return result;
}

inline Matrix& Matrix::operator-=(const Matrix& m)
{
    subtract(m);
    return *this;
}

inline const Matrix Matrix::operator-() const
{
    Matrix m(*this);
    m.negate();
    return m;
}

inline const Matrix Matrix::operator*(const Matrix& m) const
{
    Matrix result(*this);
    result.multiply(m);
    return result;
}

inline Matrix& Matrix::operator*=(const Matrix& m)
{
    multiply(m);
    return *this;
}

inline Vec3& operator*=(Vec3& v, const Matrix& m)
{
    m.transformVector(&v);
    return v;
}

inline const Vec3 operator*(const Matrix& m, const Vec3& v)
{
    Vec3 x;
    m.transformVector(v, &x);
    return x;
}

inline Vec4& operator*=(Vec4& v, const Matrix& m)
{
    m.transformVector(&v);
    return v;
}

inline const Vec4 operator*(const Matrix& m, const Vec4& v)
{
    Vec4 x;
    m.transformVector(v, &x);
    return x;
}

