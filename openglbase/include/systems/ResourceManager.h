#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "core/ISystem.h"
#include "renderer/types/UniformBuffer.h"
#include <filesystem>
#include <memory>
#include <mutex>
#include <functional>
#include <unordered_map>
#include <string>
#include <iostream>

class ResourceManager : public ISystem
{
public:
    using ResourcePtr = std::shared_ptr<Resource>;
    using ResourceCreator = std::function<ResourcePtr(const std::string&)>;

    static ResourceManager& GetInstance()
    {
        static std::unique_ptr<ResourceManager> instance(new ResourceManager());
        return *instance;
    }

    // Get a resource as a specific type
    template<typename T>
    std::shared_ptr<T> GetResourcePtr(const ResourcePtr& resource) const 
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource");
        return std::dynamic_pointer_cast<T>(resource);
    }

    // Updated registration with better type safety
    template<typename T>
    void RegisterResourceType()
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource");

        std::lock_guard<std::mutex> lock(m_Mutex);
        auto factory = [](const std::string& path) -> ResourcePtr
        {
            auto resource = std::make_shared<T>(path);
            return std::static_pointer_cast<Resource>(resource);
        };

        m_ResourceCreators[typeid(T).name()] = factory;
    }

    // Enhanced CreateOrRetrieve with type checking
    template<typename T>
    std::shared_ptr<T> LoadResource(const std::string& path)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource");

        auto typeName = typeid(T).name();
        auto fileName = std::filesystem::path(path).filename().string();
        auto key = std::string(typeName) + '_' + fileName; // Key based only on type and filename

        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto it = m_Resources.find(key);
            if (it != m_Resources.end())
            {
                std::cout << "      Resource already loaded: " << key << std::endl;
                // Return the existing resource
                return std::dynamic_pointer_cast<T>(it->second);
            }
        }

        // If the resource is not found, create a new one
        auto creatorIt = m_ResourceCreators.find(typeName);
        if (creatorIt == m_ResourceCreators.end())
        {
            std::cerr << "Unknown resource type: " << typeName << std::endl;
            throw std::runtime_error("Unknown resource type: " + std::string(typeName));
        }

        // Create the resource
        auto resource = creatorIt->second(path);

        if (resource) {
            std::cout << "      Resource created: " << key << std::endl;
            resource->Load();
        }

        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Resources[key] = resource; // Store the resource in the map
        }

        return std::dynamic_pointer_cast<T>(resource);
    }


    // Unload a resource
    void Unload(const std::string& type, const std::string& name) {
        auto key = type + ":" + name;
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto it = m_Resources.find(key);
        if (it != m_Resources.end()) {
            it->second->Unload();
            m_Resources.erase(it); // Remove from map after unloading
        }
    }

    // Remove a resource without unloading
    void Remove(const std::string& type, const std::string& name) {
        auto key = type + ":" + name;
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Resources.erase(key);
    }

    std::string GenerateResourceKey(const std::string& typeName, const std::string& path) const {
    auto fileName = std::filesystem::path(path).filename().string();
    return typeName + "_" + fileName;
}
    // Print all loaded resources
    void PrintResources() const {
        std::lock_guard<std::mutex> lock(m_Mutex);
        std::cout << "[ CURRENTLY LOADED RESOURCES ]" << std::endl;
        std::cout << "  Total Resources = " << m_Resources.size() << std::endl;
        for (const auto& pair : m_Resources) 
        {
            const std::string& key = pair.first;
            const ResourcePtr& resource = pair.second;
            if (resource) 
            {
                //std::cout << "Key: " << key << std::endl;
                std::cout << "  Path: " << resource->GetResourcePath() << std::endl;
            }
        }
    }

    void Update(float deltaTime) override {}

private:
    ResourceManager() = default;
    mutable std::mutex m_Mutex;
    std::unordered_map<std::string, ResourcePtr> m_Resources;
    std::unordered_map<std::string, ResourceCreator> m_ResourceCreators;

};

#endif // RESOURCE_MANAGER_H