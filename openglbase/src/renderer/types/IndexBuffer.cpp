#include "renderer/types/IndexBuffer.h"
#include <iostream>

IndexBuffer::IndexBuffer(const unsigned int* data, size_t count) : m_Count(count)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}
void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}
void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}