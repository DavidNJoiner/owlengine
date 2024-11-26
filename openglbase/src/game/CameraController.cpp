#include "renderer/CameraController.h"
#include "renderer/Camera.h"
#include "renderer/math/Math.h"

#include "systems/InputManager.h"

CameraController::CameraController(Camera* camera, const Vec3& target, const Vec3& offset)
    : m_Camera(camera),
    m_Target(target),
    m_Offset(offset),
    m_MovementSpeed(10.0f),       
    m_MouseSensitivity(0.7f),   
    m_FirstMouse(true),
    m_LastX(0.0f),
    m_LastY(0.0f)
{
    m_Transform = camera->GetTransform();
    m_InputManager = &InputManager::GetInstance();
}

void CameraController::Update(float deltaTime)
{
    if (!m_Camera) return;

    ProcessKeyboard(deltaTime);
    ProcessMouseMovement();
    m_Camera->OnTransformUpdated();
}

void CameraController::ProcessKeyboard(float deltaTime)
{
    if (!m_Transform || !m_InputManager || !m_Camera)
        return;

    float velocity = m_MovementSpeed * deltaTime;
    Vec3 currentPosition = m_Transform->GetPosition();
    Vec3 targetPosition = currentPosition;

    if (m_InputManager->IsKeyHeld(OWL_KEY_W))
        targetPosition += m_Camera->GetFront() * velocity;
    if (m_InputManager->IsKeyHeld(OWL_KEY_S))
        targetPosition -= m_Camera->GetFront() * velocity;
    if (m_InputManager->IsKeyHeld(OWL_KEY_A))
        targetPosition -= m_Camera->GetRight() * velocity;
    if (m_InputManager->IsKeyHeld(OWL_KEY_D))
        targetPosition += m_Camera->GetRight() * velocity;
    if (m_InputManager->IsKeyHeld(OWL_KEY_Q))
        targetPosition += m_Camera->GetUp() * velocity;
    if (m_InputManager->IsKeyHeld(OWL_KEY_E))
        targetPosition -= m_Camera->GetUp() * velocity;

    // Interpolate between current and target position
    Vec3 smoothedPosition;
    Vec3::lerp(currentPosition, targetPosition, 0.1f, &smoothedPosition);

    // Set the smoothed position
    m_Transform->SetPosition(smoothedPosition);
    m_Camera->m_ViewDirty = true; // Mark the view as dirty
}


void CameraController::ProcessMouseMovement()
{
    float xoffset, yoffset;
    m_InputManager->GetMouseDelta(xoffset, yoffset);

    if (xoffset == 0.0f && yoffset == 0.0f) return;

    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    // Set target yaw and pitch
    float targetYaw = m_Camera->m_Yaw + xoffset;
    float targetPitch = m_Camera->m_Pitch + yoffset;

    // Constrain pitch
    targetPitch = Math::clamp(targetPitch, -89.0f, 89.0f);

    // Interpolate yaw and pitch
    m_Camera->m_Yaw = Math::lerp(m_Camera->m_Yaw, targetYaw, 0.2f); // Smoothing factor
    m_Camera->m_Pitch = Math::lerp(m_Camera->m_Pitch, targetPitch, 0.2f);

    // Update the camera orientation
    UpdateCameraOrientation();
}


void CameraController::UpdateCameraOrientation()
{
    // Calculate the new front vector
    float radYaw = MATH_DEG_TO_RAD(m_Camera->m_Yaw);
    float radPitch = MATH_DEG_TO_RAD(m_Camera->m_Pitch);

    m_Camera->m_Front.x = cos(radYaw) * cos(radPitch);
    m_Camera->m_Front.y = sin(radPitch);
    m_Camera->m_Front.z = sin(radYaw) * cos(radPitch);
    m_Camera->m_Front.normalize();

    // Update right and up vectors
    Vec3::cross(m_Camera->m_Front, m_Camera->m_WorldUp, &m_Camera->m_Right);
    m_Camera->m_Right.normalize();
    Vec3::cross(m_Camera->m_Right, m_Camera->m_Front, &m_Camera->m_Up);
    m_Camera->m_Up.normalize();
}

void CameraController::SetZoom(float zoom)
{ 
    m_Camera->SetZoom(zoom); 
}