#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <iostream>
#include <string>
#include <vector>
// GLAD
#include <glad/glad.h>
#include "stb_image.h"
#include "core/ITexture.h"
#include "renderer/types/Texture2D.h"

class CubeMap : public Resource
{
public:
    CubeMap(const std::vector<std::shared_ptr<Texture2D>> cubemapTextures);
    CubeMap() = default;
    ~CubeMap();

    void Bind();
    void Unbind();

    inline GLuint GetCubeMapTextureID() const { return m_ID; }

private:
    bool Load() override;
    void Unload() override;

private:
    GLuint m_ID = 0;
    std::vector<std::shared_ptr<Texture2D>> m_CubeMapTextures;
};

#endif // CUBEMAP_H