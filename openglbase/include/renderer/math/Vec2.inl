#include "Vec2.h"

inline const Vec2 Vec2::operator+(const Vec2& v) const
{
    Vec2 result(*this);
    result.add(v);
    return result;
}

inline Vec2& Vec2::operator+=(const Vec2& v)
{
    add(v);
    return *this;
}

inline const Vec2 Vec2::operator-(const Vec2& v) const
{
    Vec2 result(*this);
    result.subtract(v);
    return result;
}

inline Vec2& Vec2::operator-=(const Vec2& v)
{
    subtract(v);
    return *this;
}

inline const Vec2 Vec2::operator-() const
{
    Vec2 result(*this);
    result.negate();
    return result;
}

inline const Vec2 Vec2::operator*(float x) const
{
    Vec2 result(*this);
    result.scale(x);
    return result;
}

inline Vec2& Vec2::operator*=(float x)
{
    scale(x);
    return *this;
}

inline const Vec2 Vec2::operator/(const float x) const
{
    return Vec2(this->x / x, this->y / x);
}

inline bool Vec2::operator<(const Vec2& v) const
{
    if (x == v.x)
    {
        return y < v.y;
    }
    return x < v.x;
}

inline bool Vec2::operator==(const Vec2& v) const
{
    return x == v.x && y == v.y;
}

inline bool Vec2::operator!=(const Vec2& v) const
{
    return x != v.x || y != v.y;
}

inline const Vec2 operator*(float x, const Vec2& v)
{
    Vec2 result(v);
    result.scale(x);
    return result;
}

