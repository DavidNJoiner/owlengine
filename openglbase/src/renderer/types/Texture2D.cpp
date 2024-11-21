#include "renderer/types/Texture2D.h"
#include "systems/ResourceManager.h"
#include "stb_image.h"

Texture2D::Texture2D(const std::string& path, int slotID)
    : ITexture(), m_SlotID(slotID), m_Width(0), m_Height(0), m_nrChannels(0)
{
    SetResourcePath(path);
}

Texture2D::~Texture2D() 
{
    Unload();
}

bool Texture2D::Load()
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(m_Path.c_str(), &m_Width, &m_Height, &m_nrChannels, 0);
    if (!image)
    {
        std::cerr << "TEXTURE_LOADING::FAILED_TO_LOAD_TEXTURE " << m_Path << std::endl;
        return false;
    }
    else
    {
        std::cout << "      Loaded texture: " << m_Path << " (" << m_Width << "x" << m_Height << ", " << m_nrChannels << " channels)" << std::endl;
    }

    GLint imageFormat = (m_nrChannels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, m_Width, m_Height, 0, imageFormat, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
    return true;
}

void Texture2D::Bind()
{
    glActiveTexture(GL_TEXTURE0 + m_SlotID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Unload() 
{
    if (m_ID != 0) {
        glDeleteTextures(1, &m_ID); // Delete the texture
        m_ID = 0; // Reset the texture ID
    }
}