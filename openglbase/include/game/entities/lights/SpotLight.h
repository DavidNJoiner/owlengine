#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "ILightEntity.h"

class SpotLight : public ILightEntity {
public:
    SpotLight(const Vec3& position, const Vec3& direction, const Vec3& color, float cutOff, float outerCutOff, std::shared_ptr<Shader> shader)
        : ILightEntity(position, color, shader), m_Direction(direction), m_CutOff(cutOff), m_OuterCutOff(outerCutOff) {}

    void Update(float deltaTime) override {
        // Update logic for spotlight if needed
    }

    const Vec3& GetDirection() const { return m_Direction; }
    float GetCutOff() const { return m_CutOff; }
    float GetOuterCutOff() const { return m_OuterCutOff; }

private:
    Vec3 m_Direction;
    float m_CutOff;
    float m_OuterCutOff;
};

#endif // SPOTLIGHT_H