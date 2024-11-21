#ifndef IENTITY_H
#define IENTITY_H

#include "pch.h"
#include "core/IComponent.h"

const size_t MAX_COMPONENTS = 32;

class Transform;
class IEntity {
public:
    using ComponentMask = std::bitset<MAX_COMPONENTS>;
    using ComponentMap = std::unordered_map<std::type_index, std::shared_ptr<IComponent>>;

    IEntity() { m_ID = s_NextID++; }
    IEntity(const IEntity&) = delete;
    ~IEntity() = default;

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        // Create the component using make_shared
        auto component = std::make_shared<T>(std::forward<Args>(args)...);

        // Store the component with the type_index of T, casting it to shared_ptr<IComponent>
        m_Components[std::type_index(typeid(T))] = std::static_pointer_cast<IComponent>(component);

        // Update the component mask
        m_ComponentMask.set(IComponent::GetComponentTypeId<T>());

        // Set the owner of the component
        component.get()->SetOwner(this);

        // Return the raw pointer to the original type
        return component.get();
    }


    template<typename T>
    T* GetComponent() const 
    {
        auto it = m_Components.find(std::type_index(typeid(T)));
        return it != m_Components.end() ? static_cast<T*>(it->second.get()) : nullptr;
    }

    template<typename T>
    void RemoveComponent() 
    {
        m_Components.erase(std::type_index(typeid(T)));
        m_ComponentMask.reset(IComponent::GetComponentTypeId<T>());
    }

    bool HasComponent(std::type_index type) const 
    {
        return m_Components.find(type) != m_Components.end();
    }

    const ComponentMask& GetComponentMask() const { return m_ComponentMask; }
    int getId() const { return m_ID; }

private:
    int m_ID = 0;
    static int s_NextID;
    ComponentMap m_Components;
    ComponentMask m_ComponentMask;
};

#endif