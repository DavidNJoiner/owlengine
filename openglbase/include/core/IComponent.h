#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "pch.h"

class IEntity;
class Transform;

class IComponent {
public:
    IEntity* GetOwner() const { return m_Owner; }
    void SetOwner(IEntity* owner) { this->m_Owner = owner; }
    virtual ~IComponent() = default;

    Transform* GetTransform() const;

    template<typename T>
    static size_t GetComponentTypeId() {
        return s_NextComponentTypeId++;
    }

    template<typename T, typename... Args>
    T* AddSubComponent(Args&&... args) {
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        m_SubComponents[std::type_index(typeid(T))] = component;
        return static_cast<T*>(component.get());
    }

    template<typename T>
    T* GetSubComponent() const {
        auto it = m_SubComponents.find(std::type_index(typeid(T)));
        return it != m_SubComponents.end() ? static_cast<T*>(it->second.get()) : nullptr;
    }

    // New functions to remove subcomponents
    template<typename T>
    bool RemoveSubComponent() {
        return m_SubComponents.erase(std::type_index(typeid(T))) > 0;
    }

    void RemoveAllSubComponents() {
        m_SubComponents.clear();
    }

protected:
    IEntity* m_Owner = nullptr;

private:
    static size_t s_NextComponentTypeId;
    std::unordered_map<std::type_index, std::shared_ptr<IComponent>> m_SubComponents;
    friend class IEntity; // Allow interaction with IEntity private member variables
};

#endif
