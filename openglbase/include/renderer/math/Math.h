#ifndef MATH_H
#define MATH_H

// Defines
#ifndef M_1_PI        
#define M_1_PI                      0.31830988618379067154
#endif
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOOVER2               1.57079632679489661923f
#define MATH_PIOOVER4               M_PI_4
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_DEG_TO_RAD(x)          ((x) * 0.01745329251994329576923690768489f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f) // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX) // Returns a random float between 0 and 1.

#define MATRIX_SIZE ( sizeof(float) * 16)

class MathFunc
{
	friend class Matrix;
	friend class Vector3;
public:
	inline static void addMatrix(const float* m, float scalar, float* dst);
	inline static void addMatrix(const float* m1, const float* m2, float* dst);
	inline static void subtractMatrix(const float* m1, const float* m2, float* dst);
	inline static void multiplyMatrix(const float* m, float scalar, float* dst);
	inline static void multiplyMatrix(const float* m1, const float* m2, float* dst);
	inline static void negateMatrix(const float* m, float* dst);
	inline static void transposeMatrix(const float* m, float* dst);
	inline static void transformVector4(const float* m, float x, float y, float z, float w, float* dst);
	inline static void transformVector4(const float* m, const float* v, float* dst);
	inline static void crossVector3(const float* v1, const float* v2, float* dst);
};

namespace Math
{
    // Clamp function
    template <typename T>
    inline T clamp(T value, T minVal, T maxVal)
    {
        if (value < minVal) return minVal;
        if (value > maxVal) return maxVal;
        return value;
    }

    // Linear interpolation function
    inline float lerp(float start, float end, float t)
    {
        t = clamp(t, 0.0f, 1.0f); // Use clamp here
        return start + t * (end - start);
    }

    inline double lerp(double start, double end, double t)
    {
        t = clamp(t, 0.0, 1.0); // Use clamp here
        return start + t * (end - start);
    }

}


#include "Math.inl"

#endif // MATH_H