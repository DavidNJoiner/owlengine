#ifndef DATALOADER_H
#define DATALOADER_H

#include "core/pch.h"
#include "renderer/Model.h"

class Transform;
class IComponent;

class DataLoader {
public:
    // Public method to access the single instance of DataLoader
    static DataLoader& GetInstance() {
        static DataLoader instance;
        return instance;
    }

    // Static methods to generate and save models
    /*
    void WriteModel(const std::string& filePath, const std::vector<Mesh>& meshes,
        const std::vector<Transform>& meshTransforms,
        const Transform& modelTransform, const std::vector<IComponent*>& components) {
        Model model(m_FilePath,);

        // Add meshes and their transforms to the model
        for (size_t i = 0; i < meshes.size(); ++i) {
            model.AddMesh(meshes[i], meshTransforms[i]);
        }

        // Save the model to a custom file format
        SaveModelToFile(filePath, model, modelTransform, components);
    }
    */

private:
    // Private constructor to prevent direct instantiation
    DataLoader() = default;

    // Delete copy constructor and assignment operator
    DataLoader(const DataLoader&) = delete;
    DataLoader& operator=(const DataLoader&) = delete;

    // Private method to save the model to a file
    void SaveModelToFile(const std::string& filePath, const Model& model,
        const Transform& modelTransform, const std::vector<IComponent*>& components) {
        // Implement the logic to write the model data to a file
        // This could involve serializing the model's meshes, transforms, and components
    }
};

#endif // DATALOADER_H
