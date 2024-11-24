#ifndef MODEL_H
#define MODEL_H

#include <iostream>

#include "stb_image.h"

#include "types/Shader.h"
#include "types/VertexBuffer.h"
#include "types/IndexBuffer.h"
#include "types/VertexArray.h"
#include "types/UniformBuffer.h"

#include "renderer/Mesh.h"

class Model : public IComponent, public Resource
{
public:
    // Instanciate a Model
    Model(const std::string& path);
    // Instanciate a new Model from a Mesh
    Model(const std::shared_ptr<Mesh>& meshResource);

    void Render(const std::shared_ptr<Shader>& shader, const ViewUniforms& viewUniform);
    void Unbind();

    inline const void SetBufferUsage(BufferUsage usage) { m_Usage = usage; }
    inline const void AddMesh(std::shared_ptr<Mesh>& mesh) { m_Meshes.push_back(mesh); }
    inline const Model* GetModel() { return this; };
    inline const Vec3& GetPosition() { return GetSubComponent<Transform>()->GetPosition(); }
    inline const Matrix& GetTransformMatrix() { return GetSubComponent<Transform>()->GetTransformMatrix(); }

    inline void SetPosition(const Vec3& pos) { GetSubComponent<Transform>()->SetPosition(pos); }
    const std::string& GetPath() const { return m_Path; }
    

private:
    bool Load() override;
    void Unload() override;
    
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::shared_ptr<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
    BufferUsage m_Usage = BufferUsage::None;
    Transform* m_Transform = nullptr;
    std::vector<std::shared_ptr<Texture2D>> m_Textures;
    std::vector<std::shared_ptr<Mesh>> m_Meshes;
    std::string m_InstanceID;
    static double probabilityOfMissing;
};
#endif