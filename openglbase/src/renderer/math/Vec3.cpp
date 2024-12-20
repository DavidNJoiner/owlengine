#include "renderer/math/Vec3.h"
#include <renderer/math/math.h>
#include "core/pch.h"

Vec3::Vec3()
    : x(0.0f), y(0.0f), z(0.0f)
{
}

Vec3::Vec3(float x, float y, float z)
    : x(x), y(y), z(z)
{
}

Vec3::Vec3(float p)
    : x(p), y(p), z(p)
{
}

Vec3::Vec3(const float* array)
{
    set(array);
}

Vec3::Vec3(const Vec3& p1, const Vec3& p2)
{
    set(p1, p2);
}

Vec3::Vec3(const Vec3& copy)
{
    set(copy);
}

Vec3 Vec3::fromColor(unsigned int color)
{
    float components[3];
    int componentIndex = 0;
    for (int i = 2; i >= 0; --i)
    {
        int component = (color >> i * 8) & 0x0000ff;

        components[componentIndex++] = static_cast<float>(component) / 255.0f;
    }

    Vec3 value(components);
    return value;
}

Vec3::~Vec3()
{
}

const Vec3& Vec3::zero()
{
    static Vec3 value(0.0f, 0.0f, 0.0f);
    return value;
}

const Vec3& Vec3::one()
{
    static Vec3 value(1.0f, 1.0f, 1.0f);
    return value;
}

const Vec3& Vec3::unitX()
{
    static Vec3 value(1.0f, 0.0f, 0.0f);
    return value;
}

const Vec3& Vec3::unitY()
{
    static Vec3 value(0.0f, 1.0f, 0.0f);
    return value;
}

const Vec3& Vec3::unitZ()
{
    static Vec3 value(0.0f, 0.0f, 1.0f);
    return value;
}

bool Vec3::isZero() const
{
    return x == 0.0f && y == 0.0f && z == 0.0f;
}

bool Vec3::isOne() const
{
    return x == 1.0f && y == 1.0f && z == 1.0f;
}

float Vec3::angle(const Vec3& v1, const Vec3& v2)
{
    float dx = v1.y * v2.z - v1.z * v2.y;
    float dy = v1.z * v2.x - v1.x * v2.z;
    float dz = v1.x * v2.y - v1.y * v2.x;

    return atan2f(sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, dot(v1, v2));
}

void Vec3::add(const Vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vec3::add(const Vec3& v1, const Vec3& v2, Vec3* dst)
{
    ASSERT(dst);

    dst->x = v1.x + v2.x;
    dst->y = v1.y + v2.y;
    dst->z = v1.z + v2.z;
}

void Vec3::clamp(const Vec3& min, const Vec3& max)
{
    ASSERT(!(min.x > max.x || min.y > max.y || min.z > max.z));

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

    // Clamp the z value.
    if (z < min.z)
        z = min.z;
    if (z > max.z)
        z = max.z;
}

void Vec3::clamp(const Vec3& v, const Vec3& min, const Vec3& max, Vec3* dst)
{
    ASSERT(dst);
    ASSERT(!(min.x > max.x || min.y > max.y || min.z > max.z));

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

    // Clamp the z value.
    dst->z = v.z;
    if (dst->z < min.z)
        dst->z = min.z;
    if (dst->z > max.z)
        dst->z = max.z;
}

void Vec3::cross(const Vec3& v)
{
    cross(*this, v, this);
}

void Vec3::cross(const Vec3& v1, const Vec3& v2, Vec3* dst)
{
    ASSERT(dst);

    // NOTE: This code assumes Vec3 struct members are contiguous floats in memory.
    // We might want to revisit this (and other areas of code that make this assumption)
    // later to guarantee 100% safety/compatibility.
    MathFunc::crossVector3(&v1.x, &v2.x, &dst->x);
}

float Vec3::distance(const Vec3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

float Vec3::distanceSquared(const Vec3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return (dx * dx + dy * dy + dz * dz);
}

float Vec3::dot(const Vec3& v) const
{
    return (x * v.x + y * v.y + z * v.z);
}

float Vec3::dot(const Vec3& v1, const Vec3& v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float Vec3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

float Vec3::lengthSquared() const
{
    return (x * x + y * y + z * z);
}

void Vec3::negate()
{
    x = -x;
    y = -y;
    z = -z;
}

Vec3& Vec3::normalize()
{
    normalize(this);
    return *this;
}

void Vec3::normalize(Vec3* dst) const
{
    ASSERT(dst);

    if (dst != this)
    {
        dst->x = x;
        dst->y = y;
        dst->z = z;
    }

    float n = x * x + y * y + z * z;
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
    dst->z *= n;
}

void Vec3::scale(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

void Vec3::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vec3::set(const float* array)
{
    ASSERT(array);

    x = array[0];
    y = array[1];
    z = array[2];
}

void Vec3::set(const Vec3& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

void Vec3::set(const Vec3& p1, const Vec3& p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
    z = p2.z - p1.z;
}

void Vec3::subtract(const Vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vec3::subtract(const Vec3& v1, const Vec3& v2, Vec3* dst)
{
    ASSERT(dst);

    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
    dst->z = v1.z - v2.z;
}

void Vec3::smooth(const Vec3& target, float elapsedTime, float responseTime)
{
    if (elapsedTime > 0)
    {
        *this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
    }
}

void Vec3::lerp(const Vec3& start, const Vec3& end, float t, Vec3* dst)
{
    ASSERT(dst);

    t = std::max(0.0f, std::min(1.0f, t));  // Clamp t between 0 and 1
    dst->x = start.x + t * (end.x - start.x);
    dst->y = start.y + t * (end.y - start.y);
    dst->z = start.z + t * (end.z - start.z);
}
