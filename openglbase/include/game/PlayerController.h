#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "core/IComponent.h"
#include "systems/InputManager.h"
#include "renderer/Transform.h"
#include "renderer/math/Vec3.h"
#include "renderer/math/Quaternion.h"

class Player;
class InputManager;

class PlayerController : public IComponent {
public:
    PlayerController(Player* player);

    void Update(float deltaTime);
    void ProcessInput(float deltaTime);

    const Vec3& GetPosition() const;
    const Quaternion& GetRotation() const;
    void SetPosition(Vec3& position);
    void SetRotation(Quaternion& rotation);

    void ProcessKeyboardInput(float deltaTime);
    void ProcessMouseInput();

private:
    
    Player* m_Player;
    Transform* m_Transform;
    InputManager* m_InputManager;
    float m_MouseSensitivity = 0.1f;
};

#endif // PLAYERCONTROLLER_H