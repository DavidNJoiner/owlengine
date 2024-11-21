#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "core/IEntity.h"
#include "renderer/math/Matrix.h"
#include "renderer/math/Quaternion.h"

class Transform : public IComponent
{
public:
    Transform(const Vec3& pos = Vec3(0.0f),
        const Quaternion& rot = Quaternion(0.0f, 0.0f, 0.0f, 1.0f),
        const Vec3& scl = Vec3(1.0f));

    // Move constructor
    Transform(Transform&& other) noexcept
        : m_Position(std::move(other.m_Position)),
        m_Rotation(std::move(other.m_Rotation)),
        m_Scale(std::move(other.m_Scale)),
        m_TransformMatrix(std::move(other.m_TransformMatrix)),
        m_IsDirty(other.m_IsDirty)
    {}

    // Copy constructor
    Transform(const Transform& other)
        : m_Position(other.m_Position),
        m_Rotation(other.m_Rotation),
        m_Scale(other.m_Scale),
        m_TransformMatrix(other.m_TransformMatrix),
        m_IsDirty(other.m_IsDirty)
    {}

    // Move assignment operator
    Transform& operator=(Transform&& other) noexcept
    {
        if (this != &other)
        {
            m_Position = std::move(other.m_Position);
            m_Rotation = std::move(other.m_Rotation);
            m_Scale = std::move(other.m_Scale);
            m_TransformMatrix = std::move(other.m_TransformMatrix);
            m_IsDirty = other.m_IsDirty;
        }
        return *this;
    }

    void SetTransformMatrix(const Matrix& transform) { m_TransformMatrix = transform; }
    void SetPosition(const Vec3& pos) { m_Position = pos; m_IsDirty = true; }
    void SetRotation(const Quaternion& rot) { m_Rotation = rot; m_IsDirty = true; }
    void SetScale(const Vec3& scl) { m_Scale = scl; m_IsDirty = true; }

    inline const Vec3& GetPosition() const { return m_Position; }
    inline const Quaternion& GetRotation() const { return m_Rotation; }
    inline const Vec3& GetScale() const { return m_Scale; }

    const Matrix& GetTransformMatrix();

    // Additional getter methods
    Vec3 GetForward() const;
    Vec3 GetRight() const;
    Vec3 GetUp() const;

    // Method to get the inverse of the transform matrix (useful for view matrix calculation)
    Matrix GetInverseTransformMatrix(); 

private:
    Matrix m_TransformMatrix;
    Quaternion m_Rotation;
    Vec3 m_Position;
    Vec3 m_Scale;
    bool m_IsDirty;

    void UpdateMatrix()
    {
        m_TransformMatrix = Matrix::identity();
        m_TransformMatrix.translate(m_Position);     
        m_TransformMatrix.rotateX(m_Rotation.x);     
        m_TransformMatrix.rotateY(m_Rotation.y);
        m_TransformMatrix.rotateZ(m_Rotation.z);
        m_TransformMatrix.scale(m_Scale);           
        m_IsDirty = false;
    }
};
#endif