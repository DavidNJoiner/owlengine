#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "ILightEntity.h"

class SpotLight : public ILightEntity {
public:
    SpotLight(const Vec3& position, const Vec3& color, const Vec3& direction = Vec3(0.0f), float cutOff = cos(MATH_DEG_TO_RAD(5.5f)), float outerCutOff = cos(MATH_DEG_TO_RAD(25.5f)))
        : ILightEntity(position, color), m_Direction(direction), m_CutOff(cutOff), m_OuterCutOff(outerCutOff) {}

    void Update(float deltaTime) override {
        // Update logic for spotlight if needed
    }

    void SetSpotlightDirection(const Vec3& targetDirection)
    {
        m_Direction = targetDirection;  
        m_Direction.normalize(); 
    }

    inline const Vec3& GetDirection() const { return m_Direction; }
    inline float GetCutOff() const { return m_CutOff; }
    inline float GetOuterCutOff() const { return m_OuterCutOff; }
    inline float GetConstant() const { return m_Constant; }
    inline float GetLinear() const { return m_Linear; }
    inline float GetQuadratic() const { return m_Quadratic; }

private:
    Vec3 m_Direction;
    float m_CutOff;
    float m_OuterCutOff;
    float m_Constant = 1.0f;
    float m_Linear = 0.09f;
    float m_Quadratic = 0.032f;
};

#endif // SPOTLIGHT_H