#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include "renderer/types/VertexBuffer.h"
#include "renderer/types/VertexArray.h"
#include "renderer/debug/Color.h"
#include "renderer/debug/Line.h"
#include "renderer/Camera.h"

class Vec3;
class DebugRenderer {
public:
    static void Initialize();
    static void SetCamera(Camera* camera);
    static void AddLine(Line* line);
    static void RemoveLine(Line* line);
    static void Render();

private:
    static VertexBufferLayout m_debugVertexlayout;
    static std::shared_ptr<Shader> m_shader_debugline;
    static std::shared_ptr<Shader> m_shader_worldgrid;
    static std::vector<Line*> s_lines;
    static std::vector<DebugVertex> vertexData;
    static std::unique_ptr<VertexArray> va;
    static std::unique_ptr<VertexBuffer> vb;
    static Camera* m_camera;
};

#endif //DEBUG_RENDERER_H