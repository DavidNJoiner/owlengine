#include "renderer/types/VertexArray.h"
#include <iostream>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
    if (m_RendererID == 0) {
        std::cerr << "Error generating Vertex Array Object." << std::endl;
    }
    glBindVertexArray(m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // Bind this VAO
    Bind(); 
    // Binds the VBO to this VAO
    vb.Bind();
    // Setup the Array Buffer Layout
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        // see this : (void*)offsetof(Vertex, Normal)); might make for a better architecture ?
        glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)(size_t)offset);
        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}