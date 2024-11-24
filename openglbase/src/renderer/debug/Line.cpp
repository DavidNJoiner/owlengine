#include "renderer/debug/DebugRenderer.h"
#include "renderer/debug/Color.h"
#include "renderer/debug/Line.h"

// Constructor with start, end, color, and thickness
Line::Line(const DebugVertex& start, const DebugVertex& end, const Color& color, float thickness)
    : m_start(start), m_end(end), m_color(color), m_thickness(thickness)
{
    DebugRenderer::AddLine(this);
}

Line::~Line()
{
    DebugRenderer::RemoveLine(this);
}

void Line::Update(const Vec3& start, const Vec3& end)
{
    m_start.position = start;
    m_end.position = end;
}

void Line::SetColor(const Color& color)
{
    m_color = color;
}

void Line::SetThickness(float thickness)
{
    m_thickness = thickness;
}

void Line::Render() const
{
    //DebugRenderer::DrawLine(m_start, m_end, m_color, m_thickness);
}