#ifndef VELOCITY_H
#define VELOCITY_H

#include "core/IComponent.h"
#include "core/IEntity.h"

namespace physx
{
    class Velocity : public IComponent {
    public:
        Vec3 velocity;

        Velocity(const Vec3& vel = Vec3(0.0f)) : velocity(vel) {}
    };
}

#endif // VELOCITY_H