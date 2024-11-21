// src/renderer/CubeMap.cpp
#include "renderer/types/CubeMap.h"
#include "systems/ResourceManager.h" 

CubeMap::CubeMap(const std::vector<std::shared_ptr<Texture2D>> cubemapTextures)
{
    for (std::shared_ptr<Texture2D> texturePtr : cubemapTextures) 
    {
        m_CubeMapTextures.push_back(texturePtr);
    }
}

CubeMap::~CubeMap() 
{
    Unload();
}

// Loads Cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
bool CubeMap::Load()
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    int width, height, bpp;
    unsigned char* image;

    int texIndex = 0;
    for (const auto& texturePtr : m_CubeMapTextures)
    {
        image = stbi_load(texturePtr->GetResourcePath().c_str(), &width, &height, &bpp, 0);
        if (!image)
            std::cout << "Failed to load texture: " << texturePtr->GetResourcePath() << std::endl;
        else {
            std::cout << "Image details: " << std::endl;
            std::cout << "\tFile path: " << texturePtr->GetResourcePath() << " ID: " << texIndex << std::endl;
            std::cout << "\tDimensions: (" << width << ", " << height << ")" << std::endl;
            std::cout << "\tBits per pixel: " << bpp << std::endl;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + texIndex, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
        texIndex++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}

void CubeMap::Unload() 
{
    if (m_ID != 0) {
        glDeleteTextures(1, &m_ID); 
        m_ID = 0;
    }
}

void CubeMap::Bind() 
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void CubeMap::Unbind() 
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}