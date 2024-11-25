// src/renderer/CubeMap.cpp
#include "renderer/types/CubeMap.h"
#include "systems/ResourceManager.h" 

CubeMap::CubeMap(const std::vector<std::shared_ptr<Texture2D>>& cubemapTextures)
    : ITexture(TextureType::TEXTURE_CUBEMAP, TextureFormat::RGB)
{
    m_CubeMapTextures = cubemapTextures;
    Load();
}

CubeMap::CubeMap(const std::vector<std::string>& cubemapPaths)
    : ITexture(TextureType::TEXTURE_CUBEMAP, TextureFormat::RGB)
{
    for (const auto& path : cubemapPaths) {
        m_CubeMapTextures.push_back(std::make_shared<Texture2D>(path));
    }
    Load();
}

CubeMap::~CubeMap()
{
    // Base class destructor will handle texture deletion
}

// Move constructor
CubeMap::CubeMap(CubeMap&& other) noexcept
    : ITexture(std::move(other)),
    Resource(std::move(other)),
    m_CubeMapTextures(std::move(other.m_CubeMapTextures))
{
}

// Move assignment operator
CubeMap& CubeMap::operator=(CubeMap&& other) noexcept
{
    if (this != &other) {
        ITexture::operator=(std::move(other));
        Resource::operator=(std::move(other));

        m_CubeMapTextures = std::move(other.m_CubeMapTextures);
    }
    return *this;
}

// Copy constructor
CubeMap::CubeMap(const CubeMap& other)
    : ITexture(other),
    Resource(other),
    m_CubeMapTextures(other.m_CubeMapTextures)
{
    Load();
}

// Copy assignment operator
CubeMap& CubeMap::operator=(const CubeMap& other)
{
    if (this != &other) {
        ITexture::operator=(other);
        Resource::operator=(other);

        m_CubeMapTextures = other.m_CubeMapTextures;
        Load();
    }
    return *this;
}

bool CubeMap::Load(const std::string& path)
{
    // If a path is provided, clear existing textures and load from path
    if (!path.empty()) {
        m_CubeMapTextures.clear();
        // Assuming cubemap paths follow a specific naming convention
        // You might need to adjust this based on your specific requirements
        std::vector<std::string> suffixes = { "right", "left", "top", "bottom", "front", "back" };
        for (const auto& suffix : suffixes) {
            m_CubeMapTextures.push_back(
                std::make_shared<Texture2D>(path + "_" + suffix + ".png")
            );
        }
    }

    // Validate texture count
    if (m_CubeMapTextures.size() != 6) {
        std::cerr << "CubeMap: Incorrect number of textures. Expected 6, got "
            << m_CubeMapTextures.size() << std::endl;
        return false;
    }

    // Generate and bind cubemap texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    // Load each face
    int texIndex = 0;
    for (const auto& texturePtr : m_CubeMapTextures)
    {
        int width, height, bpp;
        unsigned char* image = stbi_load(
            texturePtr->GetResourcePath().c_str(),
            &width, &height, &bpp, 0
        );

        if (!image) {
            std::cerr << "Failed to load cubemap texture: "
                << texturePtr->GetResourcePath() << std::endl;
            return false;
        }

        std::cout << "Cubemap texture loaded: " << texturePtr->GetResourcePath() << std::endl;

        // Determine format based on channels
        GLenum format = (bpp == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + texIndex,
            0, format, width, height, 0, format,
            GL_UNSIGNED_BYTE, image
        );

        stbi_image_free(image);
        texIndex++;
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return true;
}

void CubeMap::Bind(GLuint textureUnit) const
{
    // Use base class bind method
    ITexture::Bind(textureUnit);
}

void CubeMap::Unbind() const
{
    // Use base class unbind method
    ITexture::Unbind();
}
