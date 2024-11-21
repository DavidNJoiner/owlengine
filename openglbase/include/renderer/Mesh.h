// include/renderer/Mesh.h
#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <memory>
#include <vector>

// Math
#include "renderer/math/Vec2.h"
#include "renderer/math/Vec3.h"

// Assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// Data/Object Types
#include "types/Resource.h"
#include "types/Shader.h"
#include "types/VertexBuffer.h"
#include "types/IndexBuffer.h"
#include "types/VertexArray.h"
#include "types/UniformBuffer.h"
#include "types/Texture2D.h"
#include "Transform.h"


struct Vertex
{
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexCoords;
    Vec3 Tangent;
    Vec3 Bitangent;
};

class Mesh
{
public:
    Mesh(const std::string& name,std::vector<float>& vertices, std::vector<GLuint>& indices, std::vector<std::shared_ptr<Texture2D>>& textures, BufferUsage& usage);
    Mesh(const std::string& name,std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<std::shared_ptr<Texture2D>>& textures, BufferUsage& usage);
    //Mesh(const Mesh& other);
    virtual ~Mesh();

    inline GLenum GetGLUsage(BufferUsage usage) {
        switch (usage) {
        case BufferUsage::Static:
            return GL_STATIC_DRAW;
        case BufferUsage::Dynamic:
            return GL_DYNAMIC_DRAW;
        case BufferUsage::Stream:
            return GL_STREAM_DRAW;
        default:
            return GL_STATIC_DRAW; // Default usage
        }
    }

    void Render(const std::shared_ptr<Shader>& shader, ViewUniforms& viewUniform);
    void Bind(const std::shared_ptr<Shader>& shader);
    void Unbind();
    
private:
    bool Load(GLenum usage);
    void SetupMesh();
    
public:
    std::vector<Vertex> m_MeshVertices;
    std::vector<unsigned int> m_MeshIndices;
    std::vector<std::shared_ptr<Texture2D>> m_MeshTextures; // Handy for Model creation from a loaded Mesh

    size_t m_VertexCount, m_TrisCount;
    const std::string& m_MeshName;

private:
    std::unique_ptr<VertexArray> m_VAO; 
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_EBO;
    std::unique_ptr<UniformBuffer> m_UniformBuffer;
};

#endif // MESH_H