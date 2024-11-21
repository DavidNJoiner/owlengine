#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <glad/glad.h>

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    size_t m_Count;
public:
    IndexBuffer(const unsigned int* data, size_t count);
    ~IndexBuffer();

    void Bind();
    void Unbind();

    inline size_t GetCount() const { return m_Count; }
};
#endif