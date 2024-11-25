#include "renderer/Mesh.h"
#include "systems/ResourceManager.h"

Mesh::Mesh(const std::string& name, std::vector<float>& vertices, std::vector<GLuint>& indices, std::vector<std::shared_ptr<Texture2D>>& textures, BufferUsage& bufferUsage)
    : m_MeshName(name), m_VAO(nullptr), m_VBO(nullptr), m_EBO(nullptr), m_UniformBuffer(nullptr)
{
    // Stride values: 3 for position, 3 for normal, 2 for texture
    this->m_VertexCount = vertices.size() / 8; // 8 is the stride, this variable should be held by the ITexture
    this->m_TrisCount = (indices.size() / 3);

    // Loop over the vertices vector
    for (size_t i = 0; i + 8 <= vertices.size(); i += 8)
    {
        Vertex vertex;
        vertex.Position.x = vertices[i];
        vertex.Position.y = vertices[i + 1];
        vertex.Position.z = vertices[i + 2];

        vertex.Normal.x = vertices[i + 3];
        vertex.Normal.y = vertices[i + 4];
        vertex.Normal.z = vertices[i + 5];

        vertex.TexCoords.x = vertices[i + 6];
        vertex.TexCoords.y = vertices[i + 7];

        this->m_MeshVertices.emplace_back(vertex);
    }
    this->m_MeshIndices.assign(indices.begin(), indices.end());
    this->m_MeshTextures.assign(textures.begin(), textures.end());
    Load(GetGLUsage(bufferUsage));
}

Mesh::Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<std::shared_ptr<Texture2D>>& textures, BufferUsage& bufferUsage)
    : m_MeshName(name), m_VAO(nullptr), m_VBO(nullptr), m_EBO(nullptr), m_UniformBuffer(nullptr)
{
    this->m_VertexCount = vertices.size();
    this->m_TrisCount = (indices.size() / 3);

    this->m_MeshVertices.assign(vertices.begin(), vertices.end());
    this->m_MeshIndices.assign(indices.begin(), indices.end());
    this->m_MeshTextures.assign(textures.begin(), textures.end());

    Load(GetGLUsage(bufferUsage));
}

Mesh::~Mesh()
{
    Unbind();
}

bool Mesh::Load(GLenum usage)
{
    std::cout << "  Loading Mesh with " << m_VertexCount << " vertex coordinates." << std::endl;

    if (this->m_MeshVertices.empty())
    {
        std::cerr << "Error: Attempted to load mesh with empty vertex data." << std::endl;
        return false;
    }

    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(
        &this->m_MeshVertices[0],
        this->m_MeshVertices.size() * sizeof(Vertex),
        usage
    );
    m_EBO = std::make_unique<IndexBuffer>(
        &this->m_MeshIndices[0],
        this->m_MeshIndices.size()
    );

    // Create UniformBuffer
    m_UniformBuffer = std::make_unique<UniformBuffer>(sizeof(ViewUniforms), usage, 0);

    this->SetupMesh();
    return true;
}

void Mesh::Unbind()
{
    if (m_VAO) {
        m_VAO->Unbind();
    }

    if (m_VBO) {
        m_VBO->Unbind();
    }

    if (m_EBO) {
        m_EBO->Unbind();
    }

    if (m_UniformBuffer) {
        m_UniformBuffer->Unbind();
    }

    for (auto& texture2D : m_MeshTextures) {
        if (texture2D) {
            texture2D->Unbind();
        }
    }
}

void Mesh::Bind(const std::shared_ptr<Shader>& shader)
{
    shader->Use();
    // Bind the textures
    for (size_t i = 0; i < m_MeshTextures.size(); i++) {
        m_MeshTextures[i]->Bind(); // Bind a texture
        const char* uniformName = "diffuse"; // or m_MeshTextures[i]->GetUniformName().c_str()
        shader->setUniform1i(uniformName, i);
    }
}

void Mesh::Render(const std::shared_ptr<Shader>& shader, const ViewUniforms& viewUniform)
{
    Bind(shader);
    m_UniformBuffer->UpdateViewUniforms(viewUniform);
    // Draw mesh
    m_UniformBuffer->Bind(0); // Binding Point is 0 by default
    m_VAO->Bind();
    m_EBO->Bind();
    glDrawElements(GL_TRIANGLES, (unsigned int)this->m_MeshIndices.size(), GL_UNSIGNED_INT, 0);
    m_VAO->Unbind();
    m_EBO->Unbind();
}

void Mesh::SetupMesh()
{
    m_VAO->Bind();
    m_VBO->Bind();
    m_EBO->Bind();
    VertexBufferLayout layout;
    layout.Push<float>(3); // Vertex Position
    layout.Push<float>(3); // Normal Vectors
    layout.Push<float>(2); // Texture Coordinates
    layout.Push<float>(3); // Tangent
    layout.Push<float>(3); // Bi-Tangent
    m_VAO->AddBuffer(*m_VBO, layout);
    m_VAO->Unbind();
}