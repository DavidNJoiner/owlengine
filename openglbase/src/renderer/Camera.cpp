#include "renderer/Camera.h"
#include "renderer/math/Math.h"
#include "renderer/math/Quaternion.h"
#include "renderer/CameraController.h"

Camera::Camera(const Vec3& position)
    : m_WorldUp(0.0f, 1.0f, 0.0f),
    m_Front(-1.0f, 0.0f, 0.0f), // Align with yaw = -90 degrees (pointing along negative Z)
    m_Yaw(-90.0f), m_Pitch(0.0f), m_Zoom(45.0f),
    m_ViewDirty(true), m_ProjectionDirty(true)
{
    m_Transform = AddComponent<Transform>(position);
    OnTransformUpdated();
}

const Matrix& Camera::GetViewMatrix()
{
    if (m_ViewDirty)
    {
        UpdateViewMatrix();
    }
    return m_ViewMatrix;
}

const Matrix& Camera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

void Camera::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_Fov = fov;
    m_AspectRatio = aspectRatio;
    m_NearPlane = nearPlane;
    m_FarPlane = farPlane;
    m_ProjectionDirty = true;
    UpdateProjectionMatrix();
}

void Camera::SetZoom(float zoom)
{
    m_Zoom = std::clamp(zoom, 1.0f, 45.0f);
    m_ViewDirty = true;
}

void Camera::UpdateViewMatrix()
{
    const Vec3& position = m_Transform->GetPosition();
    Vec3 target = position + m_Front;
    Matrix::createLookAt(position, target, m_Up, &m_ViewMatrix);
    m_ViewDirty = false;
    m_Frustum.set(m_ViewMatrix);
}

void Camera::UpdateProjectionMatrix()
{
    if (m_ProjectionDirty)
    {
        Matrix::createPerspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane, &m_ProjectionMatrix);
        m_ProjectionDirty = false;
    }
}

void Camera::OnTransformUpdated()
{
    // Calculate the new front vector based on yaw and pitch
    float radYaw = MATH_DEG_TO_RAD(m_Yaw);
    float radPitch = MATH_DEG_TO_RAD(m_Pitch);

    m_Front.x = cos(radYaw) * cos(radPitch);
    m_Front.y = sin(radPitch);
    m_Front.z = sin(radYaw) * cos(radPitch);
    m_Front.normalize();

    // Update right and up vectors
    Vec3::cross(m_Front, m_WorldUp, &m_Right);
    m_Right.normalize();
    Vec3::cross(m_Right, m_Front, &m_Up);
    m_Up.normalize();

    // Mark the view matrix as dirty to ensure it gets updated
    m_ViewDirty = true;
}
