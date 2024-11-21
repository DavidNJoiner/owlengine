#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "core/pch.h"
#include <glad/glad.h>
#include <unordered_map>
#include "renderer/types/Resource.h"
#include "renderer/types/UniformBuffer.h"

class Vec2;
class Vec3;
class Vec4;
class Matrix;

class Shader : public Resource
{
public:
    GLuint Program = 0;

    // Constructor generates the shader on the fly
    Shader(const std::string& path);
    virtual ~Shader(); // Define the destructor

    // Delete copy constructor and copy assignment operator
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Move constructor 
    Shader(Shader&& other) noexcept : Program(other.Program) {
        other.Program = 0; // Invalidate the moved-from object
    }

    // Move assignment operator
    Shader& operator=(Shader&& other) noexcept {
        if (this != &other) {
            // Clean up current resources if necessary
            Unload(); // Unload current shader if needed
            Program = other.Program;
            other.Program = 0; // Invalidate the moved-from object
        }
        return *this;
    }

    // Compile the current shader
    GLuint compileShader(GLenum type, const GLchar* source, const std::string& path);

    // Uses the current shader
    void Use()const;

    const std::string& GetResourcePath() const override { return m_Path; }

    void setUniform1f(const GLchar* name, float value);
    void setUniform1fv(const GLchar* name, float* value, GLsizei count);
    void setUniform1i(const GLchar* name, int value);
    void setUniform1ui(const GLchar* name, unsigned int value);
    void setUniform1iv(const GLchar* name, int* value, GLsizei count);
    void setUniform2f(const GLchar* name, const Vec2& vector);
    void setUniform3f(const GLchar* name, const Vec3& vector);
    void setUniform4f(const GLchar* name, const Vec4& vector);
    void setUniformMat4f(const char* name, const Matrix& value);
    void setViewUniforms(const ViewUniforms& uniforms);

    GLint getUniformLocation(const std::string& name);

private:
    bool Load() override;
    void Unload() override;

private:
    std::unordered_map<std::string, GLint> m_ShaderLocationCache;
    

    // Member variables to store shader paths
    const char* m_VertexPath = nullptr;
    const char* m_FragmentPath = nullptr;
    const char* m_GeometryPath = nullptr;
    const char* m_ComputePath = nullptr;
};

#endif // SHADER_H