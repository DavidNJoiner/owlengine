#ifndef PLAYER_H
#define PLAYER_H

#include "renderer/Transform.h"
#include "renderer/physx/AABB.h"
#include "renderer/physx/Velocity.h"
#include "renderer/Camera.h"
#include "renderer/Model.h"

#include "game/PlayerController.h"

#include <renderer/CameraController.h>

class Player : public IEntity {
public:
    Player(const std::string& path, const Vec3& startPosition, float health, float moveSpeed)
        : m_Health(health), m_MoveSpeed(moveSpeed)
    {
        AddComponent<Transform>(startPosition);
        //AddComponent<Model>(path, BufferUsage::Dynamic);
       
        // Physics components
        AddComponent<physx::AABB>();
        AddComponent<physx::Velocity>();

        // Controller / Inputs
        auto playerController = AddComponent<PlayerController>(this);
    }

    const Vec3 GetPosition() const 
    {
        if (auto transform = GetComponent<Transform>()) {
            return transform->GetPosition();
        }
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    void Update(std::shared_ptr<Shader> shader, ViewUniforms& viewUniform, float deltaTime)
    {
        if (auto controller = GetComponent<PlayerController>())
        {
            controller->ProcessInput(deltaTime);
            //controller->SetPosition(GetPosition());//TESTING
            controller->Update(deltaTime);
        }

        if (auto model = GetModel())
        {
            model->Render(shader, viewUniform);
        }
    }

    Model* GetModel() const { return GetComponent<Model>(); }
    float GetMoveSpeed() const { return m_MoveSpeed; }

private:
    friend class PlayerController;
    const Vec3 m_CameraOffset = { 0.0f, 2.0f, -5.0f };
    float m_Health = 100.0f;
    float m_MoveSpeed = 1.0f;
};

#endif
