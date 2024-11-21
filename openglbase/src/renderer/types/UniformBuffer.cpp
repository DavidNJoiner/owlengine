#include "renderer/types/UniformBuffer.h"

UniformBuffer::UniformBuffer(size_t size, GLenum usage, size_t bindingPoint)
    : m_Size(size), m_BindingPoint(bindingPoint) 
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, usage);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // Unbind after initialization
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void UniformBuffer::Bind(unsigned int bindingPoint) const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_RendererID); // Use the passed binding point
}

void UniformBuffer::Update(const void* data, size_t size) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data); // Update the buffer contents
}

void UniformBuffer::Unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::UpdateViewUniforms(const ViewUniforms& viewUniforms) const
{
    Update(&viewUniforms, sizeof(ViewUniforms)); 
}