#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "core/ISystem.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <type_traits>

class SystemManager {
public:
    // Singleton instance
    static SystemManager& GetInstance() {
        static SystemManager instance;
        return instance;
    }

    // Register non-singleton systems
    template<typename T, typename... Args>
    std::enable_if_t<!std::is_same<decltype(T::GetInstance()), T&>::value, T*>
        RegisterSystem(Args&&... args) {
        static_assert(std::is_base_of<ISystem, T>::value, "T must derive from ISystem");
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T* systemPtr = system.get();
        m_Systems[typeid(T).hash_code()] = std::move(system);
        m_UpdateOrder.push_back(typeid(T).hash_code());
        return systemPtr;
    }

    // Register singleton systems
    template<typename T>
    std::enable_if_t<std::is_same<decltype(T::GetInstance()), T&>::value, T*>
        RegisterSystem() {
        static_assert(std::is_base_of<ISystem, T>::value, "T must derive from ISystem");
        T* systemPtr = &T::GetInstance();
        m_SingletonSystems[typeid(T).hash_code()] = systemPtr;
        m_UpdateOrder.push_back(typeid(T).hash_code());
        return systemPtr;
    }

    template<typename T>
    T* GetSystem() {
        size_t typeId = typeid(T).hash_code();
        if (m_SingletonSystems.find(typeId) != m_SingletonSystems.end()) {
            return static_cast<T*>(m_SingletonSystems[typeId]);
        }
        else if (m_Systems.find(typeId) != m_Systems.end()) {
            return static_cast<T*>(m_Systems[typeId].get());
        }
        return nullptr;
    }

    void UpdateSystems(float deltaTime) {
        for (auto typeId : m_UpdateOrder) {
            if (m_SingletonSystems.find(typeId) != m_SingletonSystems.end()) {
                m_SingletonSystems[typeId]->Update(deltaTime);
            }
            else if (m_Systems.find(typeId) != m_Systems.end()) {
                m_Systems[typeId]->Update(deltaTime);
            }
        }
    }

private:
    std::unordered_map<size_t, std::unique_ptr<ISystem>> m_Systems;
    std::unordered_map<size_t, ISystem*> m_SingletonSystems;
    std::vector<size_t> m_UpdateOrder;
};

#endif // SYSTEM_MANAGER_H
