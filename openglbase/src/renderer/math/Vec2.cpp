#include "core/pch.h"
#include "renderer/math/math.h"
#include "renderer/math/Vec2.h"

Vec2::Vec2()
    : x(0.0f), y(0.0f)
{
}

Vec2::Vec2(float x, float y)
    : x(x), y(y)
{
}

Vec2::Vec2(const float* array)
{
    set(array);
}

Vec2::Vec2(const Vec2& p1, const Vec2& p2)
{
    set(p1, p2);
}

Vec2::Vec2(const Vec2& copy)
{
    set(copy);
}

Vec2::~Vec2()
{
}

const Vec2& Vec2::zero()
{
    static Vec2 value(0.0f, 0.0f);
    return value;
}

const Vec2& Vec2::one()
{
    static Vec2 value(1.0f, 1.0f);
    return value;
}

const Vec2& Vec2::unitX()
{
    static Vec2 value(1.0f, 0.0f);
    return value;
}

const Vec2& Vec2::unitY()
{
    static Vec2 value(0.0f, 1.0f);
    return value;
}

bool Vec2::isZero() const
{
    return x == 0.0f && y == 0.0f;
}

bool Vec2::isOne() const
{
    return x == 1.0f && y == 1.0f;
}

float Vec2::angle(const Vec2& v1, const Vec2& v2)
{
    float dz = v1.x * v2.y - v1.y * v2.x;
    return atan2f(fabsf(dz) + MATH_FLOAT_SMALL, dot(v1, v2));
}

void Vec2::add(const Vec2& v)
{
    x += v.x;
    y += v.y;
}

void Vec2::add(const Vec2& v1, const Vec2& v2, Vec2* dst)
{
    ASSERT(dst);

    dst->x = v1.x + v2.x;
    dst->y = v1.y + v2.y;
}

void Vec2::clamp(const Vec2& min, const Vec2& max)
{
    ASSERT(!(min.x > max.x || min.y > max.y));

    // Clamp the x value.
    if (x < min.x)
        x = min.x;
    if (x > max.x)
        x = max.x;

    // Clamp the y value.
    if (y < min.y)
        y = min.y;
    if (y > max.y)
        y = max.y;
}

void Vec2::clamp(const Vec2& v, const Vec2& min, const Vec2& max, Vec2* dst)
{
    ASSERT(dst);
    ASSERT(!(min.x > max.x || min.y > max.y));

    // Clamp the x value.
    dst->x = v.x;
    if (dst->x < min.x)
        dst->x = min.x;
    if (dst->x > max.x)
        dst->x = max.x;

    // Clamp the y value.
    dst->y = v.y;
    if (dst->y < min.y)
        dst->y = min.y;
    if (dst->y > max.y)
        dst->y = max.y;
}

float Vec2::distance(const Vec2& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;

    return sqrt(dx * dx + dy * dy);
}

float Vec2::distanceSquared(const Vec2& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    return (dx * dx + dy * dy);
}

float Vec2::dot(const Vec2& v) const
{
    return (x * v.x + y * v.y);
}

float Vec2::dot(const Vec2& v1, const Vec2& v2)
{
    return (v1.x * v2.x + v1.y * v2.y);
}

float Vec2::length() const
{
    return sqrt(x * x + y * y);
}

float Vec2::lengthSquared() const
{
    return (x * x + y * y);
}

void Vec2::negate()
{
    x = -x;
    y = -y;
}

Vec2& Vec2::normalize()
{
    normalize(this);
    return *this;
}

void Vec2::normalize(Vec2* dst) const
{
    ASSERT(dst);

    if (dst != this)
    {
        dst->x = x;
        dst->y = y;
    }

    float n = x * x + y * y;
    // Already normalized.
    if (n == 1.0f)
        return;

    n = sqrt(n);
    // Too close to zero.
    if (n < MATH_TOLERANCE)
        return;

    n = 1.0f / n;
    dst->x *= n;
    dst->y *= n;
}

void Vec2::scale(float scalar)
{
    x *= scalar;
    y *= scalar;
}

void Vec2::scale(const Vec2& scale)
{
    x *= scale.x;
    y *= scale.y;
}

void Vec2::rotate(const Vec2& point, float angle)
{
    float sinAngle = sin(angle);
    float cosAngle = cos(angle);

    if (point.isZero())
    {
        float tempX = x * cosAngle - y * sinAngle;
        y = y * cosAngle + x * sinAngle;
        x = tempX;
    }
    else
    {
        float tempX = x - point.x;
        float tempY = y - point.y;

        x = tempX * cosAngle - tempY * sinAngle + point.x;
        y = tempY * cosAngle + tempX * sinAngle + point.y;
    }
}

void Vec2::set(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vec2::set(const float* array)
{
    ASSERT(array);

    x = array[0];
    y = array[1];
}

void Vec2::set(const Vec2& v)
{
    this->x = v.x;
    this->y = v.y;
}

void Vec2::set(const Vec2& p1, const Vec2& p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
}

void Vec2::subtract(const Vec2& v)
{
    x -= v.x;
    y -= v.y;
}

void Vec2::subtract(const Vec2& v1, const Vec2& v2, Vec2* dst)
{
    ASSERT(dst);

    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
}

void Vec2::smooth(const Vec2& target, float elapsedTime, float responseTime)
{
    if (elapsedTime > 0)
    {
        *this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
    }
}