#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "core/IComponent.h"
#include "core/IEntity.h"
#include "game/entities/lights/ILightEntity.h"

class Scene {
public:
    void AddEntity(std::shared_ptr<IEntity> entity) {
        m_Entities.push_back(entity);
    }

    void AddLight(const std::string& type, std::shared_ptr<ILightEntity> light) {
        m_LightGroups[type].push_back(light);
    }

    // Retrieves lights of a specific type (e.g., "PointLight", "SpotLight")
    const std::vector<std::shared_ptr<ILightEntity>>& GetLights(const std::string& type) const {
        // Return an empty vector if the type is not found
        static const std::vector<std::shared_ptr<ILightEntity>> empty;
        auto it = m_LightGroups.find(type);
        return (it != m_LightGroups.end()) ? it->second : empty;
    }

    const std::vector < std::shared_ptr<IEntity>>& GetEntities() const {
        return m_Entities;
    }

private:
    std::vector<std::shared_ptr<IEntity>> m_Entities;
    std::unordered_map<std::string, std::vector<std::shared_ptr<ILightEntity>>> m_LightGroups;
};

#endif // SCENE_H