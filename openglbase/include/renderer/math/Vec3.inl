#include "Vec3.h"
#include "Matrix.h"

inline const Vec3 Vec3::operator+(const Vec3& v) const
{
    Vec3 result(*this);
    result.add(v);
    return result;
}

inline Vec3& Vec3::operator+=(const Vec3& v)
{
    add(v);
    return *this;
}

inline const Vec3 Vec3::operator-(const Vec3& v) const
{
    Vec3 result(*this);
    result.subtract(v);
    return result;
}

inline Vec3& Vec3::operator-=(const Vec3& v)
{
    subtract(v);
    return *this;
}

inline const Vec3 Vec3::operator-() const
{
    Vec3 result(*this);
    result.negate();
    return result;
}

inline const Vec3 Vec3::operator*(float x) const
{
    Vec3 result(*this);
    result.scale(x);
    return result;
}

inline Vec3& Vec3::operator*=(float x)
{
    scale(x);
    return *this;
}

inline const Vec3 Vec3::operator/(const float x) const
{
    return Vec3(this->x / x, this->y / x, this->z / x);
}

inline bool Vec3::operator<(const Vec3& v) const
{
    if (x == v.x)
    {
        if (y == v.y)
        {
            return z < v.z;
        }
        return y < v.y;
    }
    return x < v.x;
}

inline bool Vec3::operator==(const Vec3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

inline bool Vec3::operator!=(const Vec3& v) const
{
    return x != v.x || y != v.y || z != v.z;
}

inline const Vec3 operator*(float x, const Vec3& v)
{
    Vec3 result(v);
    result.scale(x);
    return result;
}