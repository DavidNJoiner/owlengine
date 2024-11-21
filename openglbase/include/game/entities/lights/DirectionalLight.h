#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "ILightEntity.h"

class DirectionalLight : public ILightEntity {
public:
    DirectionalLight(const Vec3& direction, const Vec3& color)
        : ILightEntity(Vec3(0.0f, 0.0f, 0.0f), color), m_Direction(direction) {}

    void Update(float deltaTime) override {
        // Update logic for directional light if needed
    }

    const Vec3& GetDirection() const { return m_Direction; }

private:
    Vec3 m_Direction;
};

#endif // DIRECTIONALLIGHT_H