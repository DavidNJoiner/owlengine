#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

// GLAD
#include <glad/glad.h>

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, size_t size, GLenum usage);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};
#endif