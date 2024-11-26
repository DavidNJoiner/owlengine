#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include "core/ITexture.h"
#include "renderer/types/Resource.h"

class Texture2D : public ITexture, public Resource
{
public:
    // Constructor
    Texture2D(const std::string& path = "", int slotID = 0);

    // Destructor
    ~Texture2D() override;

    // Move constructor
    Texture2D(Texture2D&& other) noexcept;

    // Move assignment operator
    Texture2D& operator=(Texture2D&& other) noexcept;

    // Copy constructor
    Texture2D(const Texture2D& other);

    // Copy assignment operator
    Texture2D& operator=(const Texture2D& other);

    // Override ITexture virtual methods
    bool Load(const std::string& path = "") override;
    void Bind(GLuint textureUnit = 0) const override;
    void Unbind() const override;

    // Additional methods
    inline int GetSlotID() const { return m_slotID; }
    inline void SetSlotID(int newSlotID) { m_slotID = newSlotID; }
    inline const char* GetUniformName() const {
        switch (m_slotID) {
        case 0: return "diffuse";
        case 1: return "specular";
        case 2: return "normal";
        case 3: return "height";
        default: return "unknown"; 
        }
    }

protected:
    // Override the pure virtual method from ITexture
    GLenum GetGLTextureTarget() const override { return GL_TEXTURE_2D; }

private:
    int m_slotID;
    int m_nrChannels;
};

#endif // TEXTURE2D_H