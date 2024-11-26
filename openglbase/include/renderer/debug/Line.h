#ifndef LINE_H
#define LINE_H

#include "renderer/math/Vec4.h"
#include "renderer/types/Shader.h"

struct DebugVertex {
    Vec3 position;
    Color color;
};

class Color;
class Line
{
public:
    // Constructor with start, end, color, and thickness
    Line(const DebugVertex& start, const DebugVertex& end, const Color& color, float thickness = 1.0f);
    ~Line();

    // Update the line's start and end points
    void Update(const Vec3& start, const Vec3& end);

    void SetColor(const Color& color);
    void SetThickness(float thickness);

    inline const DebugVertex& GetStart() { return m_start; }
    inline const DebugVertex& GetEnd() { return m_end; }
    inline const Vec4& GetColor() { return m_color.ToVec4(); }
    inline const float GetThickness() { return m_thickness; }
  

    // Method to render the line
    void Render() const;

private:
   
    DebugVertex m_start;       // Start position of the line
    DebugVertex m_end;         // End position of the line
    Color m_color;      // Color of the line
    float m_thickness;  // Thickness of the line
};

#endif // LINE_H