#include "core/IComponent.h"
#include <renderer/Transform.h>
 
size_t IComponent::s_NextComponentTypeId = 0;

Transform* IComponent::GetTransform() const
{
    return this->GetSubComponent<Transform>();
}