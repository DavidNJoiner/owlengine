#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "core/IComponent.h"
#include "core/IEntity.h"
#include "renderer/Transform.h"
#include "math/Frustum.h"
#include "math/Vec3.h"
#include "math/Matrix.h"

class Camera : public IEntity
{
public:
    Camera(const Vec3& position);

    const Matrix& GetViewMatrix();
    const Matrix& GetProjectionMatrix() const;
    void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

    float GetZoom() const { return m_Zoom; }
    void SetZoom(float zoom);
    const Vec3& GetFront() const { return m_Front; }
    const Vec3& GetUp() const { return m_Up; }
    const Vec3& GetRight() const { return m_Right; }

    const Frustum& GetFrustum() const { return m_Frustum; }
    Transform* GetTransform() const { return m_Transform; }

    void OnTransformUpdated();

private:
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();

private:
    Transform* m_Transform;
    Frustum m_Frustum;

    Vec3 m_Front;
    Vec3 m_Up;
    Vec3 m_Right;
    Vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;
    float m_Zoom;

    float m_Fov;
    float m_AspectRatio;
    float m_NearPlane;
    float m_FarPlane;

    Matrix m_ViewMatrix;
    Matrix m_ProjectionMatrix;
    bool m_ViewDirty;
    bool m_ProjectionDirty;

    friend class CameraController;
};

#endif // CAMERA_H