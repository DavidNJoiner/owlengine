#ifndef ILIGHTENTITY_H
#define ILIGHTENTITY_H

#include "core/IComponent.h"
#include "core/IEntity.h"
#include "renderer/Transform.h"
#include "renderer/math/Vec3.h"
#include "renderer/types/Shader.h"

class ILightEntity : public IEntity {
public:
    ILightEntity(const Vec3& position, const Vec3& color, const std::shared_ptr<Shader>& shader)
        :m_Color(color), m_Shader(shader)
    {
        AddComponent<Transform>(position);
    }

    virtual void Update(float deltaTime) = 0;

    std::shared_ptr<Shader> GetShader() { return m_Shader; }
    const Vec3& GetPosition() const { return GetComponent<Transform>()->GetPosition(); }
    const Vec3& GetColor() const { return m_Color; }

protected:
    Vec3 m_Color;
    const std::shared_ptr<Shader>& m_Shader;
};

#endif // ILIGHTENTITY_H