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
    Texture2D(const std::string& path, int slotID = 0);
    Texture2D() = default;
    ~Texture2D();

    // Move constructor
    Texture2D(Texture2D&& other) noexcept
        : ITexture(),
        m_SlotID(other.m_SlotID),
        m_Width(other.m_Width),
        m_Height(other.m_Height),
        m_nrChannels(other.m_nrChannels)
    {
        SetResourcePath(other.m_Path);
        SetTextureType(other.m_Type);
        m_ID = other.m_ID; // Transfer ownership of the texture ID
        other.m_ID = 0; // Invalidate the moved-from object
    }

    // Move assignment operator
    Texture2D& operator=(Texture2D&& other) noexcept
    {
        if (this != &other) {
            glDeleteTextures(1, &m_ID); // Delete the current texture
            m_ID = other.m_ID; // Transfer ownership
            other.m_ID = 0; // Invalidate the moved-from object
            m_SlotID = other.m_SlotID;
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_nrChannels = other.m_nrChannels;
            m_Type = std::move(other.m_Type);
            m_Path = std::move(other.m_Path);
        }
        return *this;
    }

    // Copy constructor
    Texture2D(const Texture2D& other)
        : ITexture(),
        m_SlotID(other.m_SlotID),
        m_Width(other.m_Width),
        m_Height(other.m_Height),
        m_nrChannels(other.m_nrChannels)
    {
        SetResourcePath(other.GetResourcePath());
        SetTextureType(other.m_Type);
    }

    // Copy assignment operator
    Texture2D& operator=(const Texture2D& other)
    {
        if (this != &other) {
            glDeleteTextures(1, &m_ID); // Delete the current texture
            m_SlotID = other.m_SlotID;
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_nrChannels = other.m_nrChannels;
            m_Type = other.m_Type;
            m_Path = other.m_Path;
            // Note: m_ID should not be copied; it should be generated anew
        }
        return *this;
    }

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
    inline void SetSlotID(int newSlotID) { m_SlotID = newSlotID; }

    // Override ITexture virtual functions
    void Bind() override;
    void Unbind() override;

private:
    bool Load() override;
    void Unload() override;

private:
    int m_SlotID;
    int m_Width;
    int m_Height;
    int m_nrChannels;
};

#endif // TEXTURE2D_H