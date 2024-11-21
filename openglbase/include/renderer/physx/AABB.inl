#include "AABB.h"

namespace physx
{

    inline AABB& AABB::operator*=(const Matrix& matrix)
    {
        transform(matrix);
        return *this;
    }

    inline const AABB operator*(const Matrix& matrix, const AABB& box)
    {
        AABB b(box);
        b.transform(matrix);
        return b;
    }

}