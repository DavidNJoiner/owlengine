#ifndef ITEXTURE_H
#define ITEXTURE_H
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <array>

class ITexture {
public:
    // Enum for different texture types
    enum class TextureType {
        TEXTURE_2D,
        TEXTURE_3D,
        TEXTURE_CUBEMAP,
        TEXTURE_ARRAY
    };

    // Enum for texture format specifications
    enum class TextureFormat {
        RGB,
        RGBA,
        DEPTH,
        LUMINANCE,
        COMPRESSED
    };

    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~ITexture() {
        if (m_textureID != 0) {
            glDeleteTextures(1, &m_textureID);
        }
    }

    // Pure virtual method to load texture (must be implemented by derived classes)
    virtual bool Load(const std::string& path) = 0;

    // Bind texture to a specific texture unit
    virtual void Bind(GLuint textureUnit = 0) const {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GetGLTextureTarget(), m_textureID);
    }

    // Unbind the texture
    virtual void Unbind() const {
        glBindTexture(GetGLTextureTarget(), 0);
    }

    // Getters
    GLuint GetTextureID() const { return m_textureID; }
    TextureType GetTextureType() const { return m_textureType; }
    TextureFormat GetTextureFormat() const { return m_textureFormat; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    void SetUniformName(const char* TextureUniformName) {}

    // Set texture parameters
    virtual void SetTextureParameters(
        GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
        GLint magFilter = GL_LINEAR,
        GLint wrapS = GL_REPEAT,
        GLint wrapT = GL_REPEAT
    ) {
        //Bind();
        glTexParameteri(GetGLTextureTarget(), GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GetGLTextureTarget(), GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GetGLTextureTarget(), GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GetGLTextureTarget(), GL_TEXTURE_WRAP_T, wrapT);
        //Unbind();
    }

    // Generate mipmaps
    virtual void GenerateMipmaps() {
        Bind();
        glGenerateMipmap(GetGLTextureTarget());
        Unbind();
    }

protected:
    // Protected constructor to prevent direct instantiation
    ITexture(TextureType type, TextureFormat format)
        : m_textureType(type), m_textureFormat(format), m_textureID(0),
        m_width(0), m_height(0) {
        // Generate texture ID
        glGenTextures(1, &m_textureID);
    }

    // Pure virtual method to get the appropriate OpenGL texture target
    virtual GLenum GetGLTextureTarget() const = 0;

    // Protected members for derived classes to use
    GLuint m_textureID;
    TextureType m_textureType;
    TextureFormat m_textureFormat;
    int m_width;
    int m_height;
};
#endif // ITEXTURE_H