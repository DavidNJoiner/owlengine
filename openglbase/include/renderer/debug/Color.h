#ifndef COLOR_H
#define COLOR_H

#include "renderer/math/Vec4.h"
#include "renderer/math/Vec3.h"

class Color
{
public:
    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}

    inline const Vec4 ToVec4() { return Vec4(r, g, b, a); }
    inline const Vec3 ToVec3() { return Vec3(r, g, b); }

    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Black;
    static const Color Orange;

    float r, g, b, a;
};

#endif // COLOR_H