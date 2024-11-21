#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "IComponent.h"

class IController : public IComponent {
public:
    virtual void Update(float deltaTime) = 0;
};

#endif