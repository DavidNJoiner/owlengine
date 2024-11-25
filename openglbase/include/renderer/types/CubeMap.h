#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include "stb_image.h"
#include "core/ITexture.h"
#include "renderer/types/Texture2D.h"
#include "renderer/types/Resource.h"

class CubeMap : public ITexture, public Resource
{
public:
    // Constructors
    CubeMap() = default;
    CubeMap(const std::vector<std::shared_ptr<Texture2D>>& cubemapTextures);
    CubeMap(const std::vector<std::string>& cubemapPaths);

    // Destructor
    ~CubeMap() override;

    // Move constructor
    CubeMap(CubeMap&& other) noexcept;

    // Move assignment operator
    CubeMap& operator=(CubeMap&& other) noexcept;

    // Copy constructor
    CubeMap(const CubeMap& other);

    // Copy assignment operator
    CubeMap& operator=(const CubeMap& other);

    // Override ITexture virtual methods
    bool Load(const std::string& path = "") override;
    void Bind(GLuint textureUnit = 0) const override;
    void Unbind() const override;

    // Additional methods
    inline GLuint GetCubeMapTextureID() const { return m_textureID; }

protected:
    // Override the pure virtual method from ITexture
    GLenum GetGLTextureTarget() const override { return GL_TEXTURE_CUBE_MAP; }

private:
    std::vector<std::shared_ptr<Texture2D>> m_CubeMapTextures;
};

#endif // CUBEMAP_H