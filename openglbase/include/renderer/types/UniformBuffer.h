#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <cstddef>
#include <glad/glad.h>
#include "renderer/math/Vec3.h"
#include "renderer/math/Matrix.h"

struct ViewUniforms 
{
    Matrix view;
    Matrix proj;
    Vec3 pos;
};

enum class BufferUsage {
    Static,  // GL_STATIC_DRAW
    Dynamic, // GL_DYNAMIC_DRAW
    Stream,   // GL_STREAM_DRAW
    None
};

class UniformBuffer
{
private:
    unsigned int m_RendererID; ///< OpenGL buffer ID
    size_t m_BindingPoint; ///< Binding point for the uniform buffer
    size_t m_Size; ///< Size of the buffer

public:
    /**
     * @brief Constructs a UniformBuffer.
     * @param size Size of the buffer in bytes.
     * @param usage OpenGL usage hint (e.g., GL_STATIC_DRAW).
     * @param bindingPoint Binding point for the uniform buffer.
     */
    UniformBuffer(size_t size, GLenum usage, size_t bindingPoint);
    ~UniformBuffer();

    void UpdateViewUniforms(const ViewUniforms& viewUniforms) const;

    void Bind(unsigned int bindingPoint) const; // Modify this line
    void Update(const void* data, size_t size) const;
    void Unbind() const;
};
#endif // UNIFORMBUFFER_H
