#include "renderer/types/Shader.h"
#include "renderer/math/Matrix.h"
#include "systems/ResourceManager.h" 

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& path)
{
    SetResourcePath(path);
}

Shader::~Shader() 
{
    Unload();
}

void Shader::Unload() 
{
    if (Program != 0) {
        glDeleteProgram(Program); // OpenGL cleanup
        Program = 0;
    }
}

bool Shader::Load() 
{  
    const char* vertexExt = ".vs";
    const char* fragmentExt = ".fs";
    const char* geometryExt = ".gs";
    const char* computeExt = ".cs";

    // Initialize paths only if m_Path is not empty
    std::string m_VertexPath = m_Path + vertexExt;
    std::string m_FragmentPath = m_Path + fragmentExt;
    std::string m_GeometryPath = m_Path + geometryExt;
    std::string m_ComputePath = m_Path + computeExt;

    // Check if vertex and fragment paths are set before loading
    if (!m_VertexPath.empty() && !m_FragmentPath.empty()) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        try {
            // Open files
            vShaderFile.open(m_VertexPath);
            fShaderFile.open(m_FragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            return false; // Exit if file reading fails
        }

        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar* fShaderCode = fragmentCode.c_str();

        //std::cout << "vertex path "<< m_VertexPath << std::endl;
        //std::cout << "fragment path " << m_FragmentPath << std::endl;
        // Compile shaders
        GLuint vertex = compileShader(GL_VERTEX_SHADER, vShaderCode, m_VertexPath);
        GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode, m_FragmentPath);

        // Create program and link shaders
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        // Print linking errors if any
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << "vertex shader " << m_VertexPath << ", fragment shader " << m_FragmentPath << " :: " << infoLog << std::endl;
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Load geometry shader if provided
    /*
    if (!m_GeometryPath.empty()) {
        std::string geometryCode;
        std::ifstream gShaderFile;

        // ensures ifstream objects can throw exceptions:
        gShaderFile.exceptions(std::ifstream::badbit);
        try {
            // Open file
            gShaderFile.open(m_GeometryPath);
            std::stringstream gShaderStream;
            // Read file's buffer contents into streams
            gShaderStream << gShaderFile.rdbuf();
            // close file handler
            gShaderFile.close();
            // Convert stream into string
            geometryCode = gShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            return false; // Exit if file reading fails
        }

        const GLchar* gShaderCode = geometryCode.c_str();

        // Compile geometry shader
        GLuint geometry = compileShader(GL_GEOMETRY_SHADER, gShaderCode, m_GeometryPath);

        // Attach geometry shader to program
        glAttachShader(this->Program, geometry);
        glDeleteShader(geometry); // Delete the geometry shader as it's linked into our program now
    }
    */

    // Bind the shader to uniform buffer block index = 0
    unsigned int viewProjectionBindingIndex = glGetUniformBlockIndex(this->Program, "VPMatrices");
    glUniformBlockBinding(this->Program, viewProjectionBindingIndex, 0);
    return true;
}

GLuint Shader::compileShader(GLenum type, const GLchar* source, const std::string& path) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : (type == GL_FRAGMENT_SHADER ? "FRAGMENT" : "GEOMETRY"))
            << "::COMPILATION_FAILED [" << path << "]\n" << infoLog << std::endl;
    }
    
    return shader;
}

GLint Shader::getUniformLocation(const std::string& name) {
    Use();
    if (m_ShaderLocationCache.find(name) != m_ShaderLocationCache.end())
        return m_ShaderLocationCache[name];

    GLint location = glGetUniformLocation(Program, name.c_str());
    m_ShaderLocationCache[name] = location;
    return location;
}

void Shader::Use() const {
    glUseProgram(Program);
}

void Shader::setUniform1f(const GLchar* name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform1fv(const GLchar* name, float* value, GLsizei count) {
    glUniform1fv(getUniformLocation(name), count, value);
}

void Shader::setUniform1i(const GLchar* name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1ui(const GLchar* name, unsigned int value) {
    glUniform1ui(getUniformLocation(name), value);
}

void Shader::setUniform1iv(const GLchar* name, int* value, GLsizei count) {
    glUniform1iv(getUniformLocation(name), count, value);
}

void Shader::setUniform2f(const GLchar* name, const Vec2& vector) {
    glUniform2f(getUniformLocation(name), vector.x, vector.y);
}

void Shader::setUniform3f(const GLchar* name, const Vec3& vector) {
    glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::setUniform4f(const GLchar* name, const Vec4& vector) {
    glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::setUniformMat4f(const char* name, const Matrix& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value.m);
}

void Shader::setViewUniforms(const ViewUniforms& uniforms) {
    Use();
    unsigned int blockIndex = glGetUniformBlockIndex(Program, "ViewUniforms");
    if (blockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(Program, blockIndex, 0);
        GLuint uboViewUniforms;
        glGenBuffers(1, &uboViewUniforms);
        glBindBuffer(GL_UNIFORM_BUFFER, uboViewUniforms);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(ViewUniforms), &uniforms, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewUniforms);
    }
    else {
        setUniformMat4f("view", uniforms.view);
        setUniformMat4f("proj", uniforms.proj);
        setUniform3f("pos", uniforms.pos);
    }
}