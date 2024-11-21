//#include "Matrix.h"
#include "Vec4.h"

inline const Vec4 Vec4::operator+(const Vec4& v) const
{
    Vec4 result(*this);
    result.add(v);
    return result;
}

inline Vec4& Vec4::operator+=(const Vec4& v)
{
    add(v);
    return *this;
}

inline const Vec4 Vec4::operator-(const Vec4& v) const
{
    Vec4 result(*this);
    result.subtract(v);
    return result;
}

inline Vec4& Vec4::operator-=(const Vec4& v)
{
    subtract(v);
    return *this;
}

inline const Vec4 Vec4::operator-() const
{
    Vec4 result(*this);
    result.negate();
    return result;
}

inline const Vec4 Vec4::operator*(float x) const
{
    Vec4 result(*this);
    result.scale(x);
    return result;
}

inline Vec4& Vec4::operator*=(float x)
{
    scale(x);
    return *this;
}

inline const Vec4 Vec4::operator/(const float x) const
{
    return Vec4(this->x / x, this->y / x, this->z / x, this->w / x);
}

inline bool Vec4::operator<(const Vec4& v) const
{
    if (x == v.x)
    {
        if (y == v.y)
        {
            if (z == v.z)
            {
                return w < v.w;
            }
            return z < v.z;
        }
        return y < v.y;
    }
    return x < v.x;
}

inline bool Vec4::operator==(const Vec4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

inline bool Vec4::operator!=(const Vec4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

inline const Vec4 operator*(float x, const Vec4& v)
{
    Vec4 result(v);
    result.scale(x);
    return result;
}