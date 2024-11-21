#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer/math/Vec4.h"

// Systems
#include "systems/InputManager.h"
#include "systems/TimeManager.h"

class Window
{
public:
    Vec4 backgroundColor;

    Window(const char* title, int width, int height);
    ~Window();

    void clear() const;
    void update() const;
    bool closed() const;

    inline float getWidth() const { return m_Width; }
    inline float getHeight() const { return m_Height; }
    inline GLFWwindow* getGLFWwindow() const { return m_Window; }

private:
    GLFWwindow* m_Window;
    float m_Width, m_Height;
    const char* m_Title;
    bool m_Closed;

private:
    bool init();
    static void glfw_initialisation_error(int error, const char* description);
    static void window_resize_callback(GLFWwindow* window, int width, int height);
};

#endif // WINDOW_H