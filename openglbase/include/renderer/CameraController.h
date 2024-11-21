#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "core/IComponent.h"
#include "renderer/math/Vec3.h"
#include "renderer/math/Matrix.h"

class Camera;
class Transform;
class InputManager;

class CameraController : public IComponent
{
public:
    CameraController(Camera* camera, const Vec3& target, const Vec3& offset = Vec3(0.f, 0.f, 0.f));

    void Update(float deltaTime);

    void SetTarget(const Vec3& target) { m_Target = target; }
    void SetOffset(const Vec3& offset) { m_Offset = offset; }
    void SetZoom(float zoom);
    const Vec3& GetOffset() const { return m_Offset; }

private:
    void ProcessKeyboard(float deltaTime);
    void ProcessMouseMovement();
    //void UpdateCameraPosition(float deltaTime);
    void UpdateCameraOrientation();

private:
    Camera* m_Camera;
    Transform* m_Transform;
    InputManager* m_InputManager;

    Vec3 m_Target;
    Vec3 m_Offset;

    float m_MovementSpeed;
    float m_MouseSensitivity;
    bool m_FirstMouse;
    float m_LastX, m_LastY;
};

#endif // CAMERACONTROLLER_H