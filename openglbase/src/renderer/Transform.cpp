#include "renderer/Transform.h"

Transform::Transform(const Vec3& pos, const Quaternion& rot, const Vec3& scl)
    : m_Position(pos), m_Rotation(rot), m_Scale(scl), m_IsDirty(true)
{
    UpdateMatrix();
}

const Matrix& Transform::GetTransformMatrix()
{
    if (m_IsDirty)
    {
        UpdateMatrix();
    }
    return m_TransformMatrix;
}

// Additional getter methods
Vec3 Transform::GetForward() const
{
    return Vec3(-m_TransformMatrix.m[2], -m_TransformMatrix.m[6], -m_TransformMatrix.m[10]).normalize();
}

Vec3 Transform::GetRight() const
{
    return Vec3(m_TransformMatrix.m[0], m_TransformMatrix.m[4], m_TransformMatrix.m[8]).normalize();
}

Vec3 Transform::GetUp() const
{
    return Vec3(m_TransformMatrix.m[1], m_TransformMatrix.m[5], m_TransformMatrix.m[9]).normalize();
}

// Method to get the inverse of the transform matrix (useful for view matrix calculation)
Matrix Transform::GetInverseTransformMatrix()
{
    if (m_IsDirty)
    {
        UpdateMatrix();
    }
    return m_TransformMatrix.invert();
}