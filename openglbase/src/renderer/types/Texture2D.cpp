#include "renderer/types/Texture2D.h"
#include "systems/ResourceManager.h"
#include "stb_image.h"

Texture2D::Texture2D(const std::string& path, int slotID)
    : ITexture(TextureType::TEXTURE_2D, TextureFormat::RGB),
    m_slotID(slotID),
    m_nrChannels(0)
{
    if (!path.empty()) {
        SetResourcePath(path);
        Load(path);
    }
}

Texture2D::~Texture2D()
{
    // Base class destructor will handle texture deletion
}

// Move constructor
Texture2D::Texture2D(Texture2D&& other) noexcept
    : ITexture(std::move(other)),
    Resource(std::move(other)),
    m_slotID(other.m_slotID),
    m_nrChannels(other.m_nrChannels)
{
    other.m_slotID = 0;
    other.m_nrChannels = 0;
}

// Move assignment operator
Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this != &other) {
        ITexture::operator=(std::move(other));
        Resource::operator=(std::move(other));

        m_slotID = other.m_slotID;
        m_nrChannels = other.m_nrChannels;

        other.m_slotID = 0;
        other.m_nrChannels = 0;
    }
    return *this;
}

// Copy constructor
Texture2D::Texture2D(const Texture2D& other)
    : ITexture(other),
    Resource(other),
    m_slotID(other.m_slotID),
    m_nrChannels(other.m_nrChannels)
{
    if (!other.GetResourcePath().empty()) {
        Load(other.GetResourcePath());
    }
}

// Copy assignment operator
Texture2D& Texture2D::operator=(const Texture2D& other)
{
    if (this != &other) {
        ITexture::operator=(other);
        Resource::operator=(other);

        m_slotID = other.m_slotID;
        m_nrChannels = other.m_nrChannels;

        if (!other.GetResourcePath().empty()) {
            Load(other.GetResourcePath());
        }
    }
    return *this;
}

bool Texture2D::Load(const std::string& path)
{
    // Use provided path or existing resource path
    std::string loadPath = path.empty() ? GetResourcePath() : path;

    if (loadPath.empty()) {
        std::cerr << "TEXTURE_LOADING::NO_PATH_PROVIDED" << std::endl;
        return false;
    }

    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(loadPath.c_str(), &m_width, &m_height, &m_nrChannels, 0);

    if (!image) {
        std::cerr << "TEXTURE_LOADING::FAILED_TO_LOAD_TEXTURE " << loadPath << std::endl;
        return false;
    }

    std::cout << "Loaded texture: " << loadPath << " ("
        << m_width << "x" << m_height << ", "
        << m_nrChannels << " channels)" << std::endl;

    // Determine format based on number of channels
    GLenum format = (m_nrChannels == 4) ? GL_RGBA : GL_RGB;

    // Bind and configure texture
    Bind();

    // Set texture parameters before loading image
    SetTextureParameters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);

    // Load texture image
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, image);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free image data
    stbi_image_free(image);

    // Unbind texture
    Unbind();

    // Update resource path if a new path was provided
    if (!path.empty()) {
        SetResourcePath(path);
    }

    return true;
}

void Texture2D::Bind(GLuint textureUnit) const
{
    // Use base class bind method
    ITexture::Bind(textureUnit); // TextureUnit for diffuse is 0 by default
}

void Texture2D::Unbind() const
{
    // Use base class unbind method
    ITexture::Unbind();
}