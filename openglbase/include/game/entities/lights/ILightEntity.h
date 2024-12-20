#ifndef ILIGHTENTITY_H
#define ILIGHTENTITY_H

#include "core/IComponent.h"
#include "core/IEntity.h"
#include "renderer/debug/Color.h"
#include "renderer/Transform.h"
#include "renderer/math/Vec3.h"
#include "renderer/types/Shader.h"

class ILightEntity : public IEntity {
public:
    ILightEntity(const Vec3& position, const Vec3& color)
        :m_Color(color)
    {
        AddComponent<Transform>(position);
    }

    virtual void Update(float deltaTime) = 0;


    inline const Vec3& GetPosition() const { return GetComponent<Transform>()->GetPosition(); }
    inline void SetPosition(Vec3& position) { GetComponent<Transform>()->SetPosition(position); }
    inline const Vec3& GetVec3Color() const { return m_Color; } 

protected:
    Vec3 m_Color;
};

#endif // ILIGHTENTITY_H