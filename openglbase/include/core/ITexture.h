#ifndef ITEXTURE_H
#define ITEXTURE_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <vector>

class ITexture
{
public:
    ITexture(): m_ID(0) {}

    virtual ~ITexture() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    const GLuint& GetTextureID() const { return m_ID; } 
    void SetTextureType(const std::string& type) { m_Type = type; }
    const std::string& GetTextureType() { return m_Type; }

protected:
    GLuint m_ID;
    std::string m_Type;
};

#endif // ITEXTURE_H