#include "renderer/debug/DebugRenderer.h"
#include "systems/ResourceManager.h"
#include "renderer/types/Shader.h"
#include "renderer/types/VertexBuffer.h"
#include "renderer/types/VertexArray.h"
#include "renderer/Camera.h"
#include "renderer/debug/Line.h"
#include <algorithm>


VertexBufferLayout DebugRenderer::m_debugVertexlayout;

// Shaders
std::shared_ptr<Shader> DebugRenderer::m_lineShader;
std::shared_ptr<Shader> DebugRenderer::m_gridShader;

std::vector<Line*> DebugRenderer::s_lines;
std::vector<DebugVertex> DebugRenderer::vertexData;
std::unique_ptr<VertexArray> DebugRenderer::va;
std::unique_ptr<VertexBuffer> DebugRenderer::vb;
Camera* DebugRenderer::m_camera = nullptr;


void DebugRenderer::Initialize()
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();

    // Load Debug Shaders
    m_lineShader = resourceManager.LoadResource<Shader>("res/shaders/debugLine");
    m_gridShader = resourceManager.LoadResource<Shader>("res/shaders/floorgrid");

    // Define the layout for DebugVertex
    m_debugVertexlayout.Push<float>(3); // position
    m_debugVertexlayout.Push<float>(4); // color
}

void DebugRenderer::SetCamera(Camera* camera)
{
    m_camera = camera;
}

void DebugRenderer::AddLine(Line* line)
{
    if (line != nullptr && std::find(s_lines.begin(), s_lines.end(), line) == s_lines.end())
    {
        s_lines.push_back(line);
    }
}

void DebugRenderer::RemoveLine(Line* line)
{
    auto it = std::find(s_lines.begin(), s_lines.end(), line);
    if (it != s_lines.end())
    {
        s_lines.erase(it);
    }
}

void DebugRenderer::Render()
{
    // RENDER DEBUG GRID ==================================================
    glDepthMask(GL_FALSE);

    GLuint quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    // Plane rendering setup
    Matrix model(1.0f);

    // Set uniforms for the grid shader
    m_gridShader->setUniform1f("near", m_camera->GetNear());
    m_gridShader->setUniform1f("far", m_camera->GetFar());
    m_gridShader->setViewUniforms(m_camera->GetViewUniform());

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);

    // RENDER DEBUG LINES ==================================================
    vertexData.clear();
    for (Line* line : s_lines)
    {
        if (line != nullptr)
        {
            DebugVertex v1 = line->GetStart();
            DebugVertex v2 = line->GetEnd();
            vertexData.push_back(v1);
            vertexData.push_back(v2);
        }
    }

    va = std::make_unique<VertexArray>();
    vb = std::make_unique<VertexBuffer>(vertexData.data(), vertexData.size() * sizeof(DebugVertex), GL_DYNAMIC_DRAW);

    va->Bind();
    va->AddBuffer(*vb.get(), m_debugVertexlayout);
    m_lineShader->Use();
    m_lineShader->setViewUniforms(m_camera->GetViewUniform());
    glDrawArrays(GL_LINES, 0, vertexData.size());
    va->Unbind();
}