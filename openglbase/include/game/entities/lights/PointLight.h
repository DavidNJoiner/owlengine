#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "ILightEntity.h"

class PointLight : public ILightEntity {
public:
    PointLight(const Vec3& position, const Vec3& color, float constant, float linear, float quadratic, const std::shared_ptr<Shader>& shader)
        : ILightEntity(position, color, shader), m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic)
    {}

    void Update(float deltaTime) override {
        // Update logic for point light if needed
    }

    float GetConstant() const { return m_Constant; }
    float GetLinear() const { return m_Linear; }
    float GetQuadratic() const { return m_Quadratic; }

private:
    float m_Constant;
    float m_Linear;
    float m_Quadratic;
};

#endif // POINTLIGHT_H