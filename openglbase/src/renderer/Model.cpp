#include "renderer/Model.h"
#include "systems/ResourceManager.h"

double Model::probabilityOfMissing = 0.8;

Model::Model(const std::string& path)
{
    m_Transform = AddSubComponent<Transform>(Vec3(0.0f)); 
    SetResourcePath(path);
}

Model::Model(const std::shared_ptr<Mesh>& meshResource)
{
    m_Transform = AddSubComponent<Transform>(Vec3(0.0f));
    m_Meshes.push_back(meshResource);
}

void Model::Render(const std::shared_ptr<Shader>& shader, const ViewUniforms& viewUniform)
{
    for (GLuint i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i]->Render(shader, viewUniform);
    }
}

bool Model::Load()
{
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(
        m_Path,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_SortByPType);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return false;
    }
    
    this->m_Path = m_Path;
    this->ProcessNode(scene->mRootNode, scene);
    return true;
}

void Model::Unload()
{
    std::cout << "[ UNLOADING MODEL ]" << std::endl;
    RemoveAllSubComponents();
    Unbind();
    m_Meshes.clear();
    m_Transform = nullptr;
}


void Model::Unbind()
{
    for (auto& mesh : m_Meshes) {
        if (mesh) {
            mesh->Unbind();
        }
    }
}

// TODO : Implement Mesh Resource Loading process using ResourceManager
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Process each mesh in the current node
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }

    // Recursively process each child node
    for (GLuint i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    // Get the mesh name
    const std::string meshName = mesh->mName.C_Str();

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        Vec3 vector;

        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // Texture coordinates
        if (mesh->mTextureCoords[0]) {
            Vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else {
            std::cout << "No texture coordinate "<< std::endl;
            vertex.TexCoords = Vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // Load different types of textures
    m_Textures.push_back(LoadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "diffuse"));
    //m_Textures.push_back(LoadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, "texture_specular"));
    //LoadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_NORMALS, "texture_normal");

    auto newMeshPtr = std::make_shared<Mesh>(meshName, vertices, indices, m_Textures, m_Usage);
    m_Meshes.push_back(newMeshPtr);
}

std::shared_ptr<Texture2D> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string texturePath = "res/textures/" + std::string(str.C_Str());

        auto texture = resourceManager.LoadResource<Texture2D>(texturePath);
        if (texture) {
            texture->SetTextureType(typeName);
            return texture;
        }
    }

    // If no texture is found, load a fallback texture
    std::string defaultTexturePath = "res/textures/debug/checker.png";
    auto defaultTexture = resourceManager.LoadResource<Texture2D>(defaultTexturePath);
    if (defaultTexture) {
        defaultTexture->SetTextureType(typeName);
    }
    return defaultTexture;
}
