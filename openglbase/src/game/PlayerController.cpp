#include <renderer/math/Math.h>
#include "game/PlayerController.h"
#include "game/Player.h"

PlayerController::PlayerController(Player* player) : m_Player(player) 
{
    AddSubComponent<Transform>();
    m_Transform = GetSubComponent<Transform>();
    m_InputManager = &InputManager::GetInstance();
}

void PlayerController::Update(float deltaTime) {
    if (m_InputManager) {
        ProcessInput(deltaTime);
    }
    // Additional update logic
}

void PlayerController::ProcessInput(float deltaTime) {
    ProcessKeyboardInput(deltaTime);
    ProcessMouseInput();
}

const Vec3& PlayerController::GetPosition() const {
    return this->m_Transform->GetPosition();
}

const Quaternion& PlayerController::GetRotation() const {
    return this->m_Transform->GetRotation();
}

void PlayerController::SetPosition(const Vec3& position) {
    this->m_Transform->SetPosition(position);
}

void PlayerController::SetRotation(const Quaternion& rotation) {
    this->m_Transform->SetRotation(rotation);
}

void PlayerController::ProcessKeyboardInput(float deltaTime)
{
    Vec3 movement(0.0f);
    if (m_InputManager->IsKeyHeld(OWL_KEY_W)) movement.z -= 1.0f;
    if (m_InputManager->IsKeyHeld(OWL_KEY_S)) movement.z += 1.0f;
    if (m_InputManager->IsKeyHeld(OWL_KEY_A)) movement.x -= 1.0f;
    if (m_InputManager->IsKeyHeld(OWL_KEY_D)) movement.x += 1.0f;

    if (movement != Vec3(0.0f)) {
        movement.normalize();

        // Create rotation matrix based on player's yaw
        float yawRadians = MATH_DEG_TO_RAD(m_Transform->GetRotation().y);
        Matrix rotationMatrix;
        Matrix::createRotationY(yawRadians, &rotationMatrix);

        // Transform movement vector by rotation matrix
        Vec3 rotatedMovement;
        rotationMatrix.transformVector(movement, &rotatedMovement);

        Vec3 newPosition = m_Transform->GetPosition() + rotatedMovement * m_Player->GetMoveSpeed() * deltaTime;
        m_Transform->SetPosition(newPosition);
    }
}

void PlayerController::ProcessMouseInput() {
    float xoffset, yoffset;
    m_InputManager->GetMouseDelta(xoffset, yoffset);

    if (xoffset != 0.0f || yoffset != 0.0f) {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        Quaternion rotation = GetRotation();
        rotation.y += xoffset;
        rotation.x -= yoffset;  // Invert Y-axis for intuitive control

        // Clamp pitch to avoid flipping
        rotation.x = std::clamp(rotation.x, -89.0f, 89.0f);

        // Ensure yaw stays within 0-360 range
        rotation.y = fmod(rotation.y, 360.0f);
        if (rotation.y < 0.0f) rotation.y += 360.0f;

        m_Transform->SetRotation(rotation);
    }
}