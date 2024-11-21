#ifndef ISYSTEM_H
#define ISYSTEM_H

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void Update(float deltaTime) = 0;
};

#endif