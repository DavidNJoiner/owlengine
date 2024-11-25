#include "renderer/Window.h"
#include <iostream>

Window::Window(const char* title, int width, int height)
    : backgroundColor(Vec4(0.1f, 0.1f, 0.1f, 1.0f)), m_Title(title), m_width((float)width), m_height((float)height),
    m_Window(nullptr), m_Closed(false)
{
    if (!init())
        glfwTerminate();
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::init()
{
    glfwSetErrorCallback(glfw_initialisation_error);
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(m_width, m_height, m_Title, NULL, NULL);
    if (m_Window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, window_resize_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void Window::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
}

void Window::update() const
{
    glfwSwapBuffers(m_Window);
}

bool Window::closed() const
{
    return glfwWindowShouldClose(m_Window);
}

void Window::glfw_initialisation_error(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void Window::window_resize_callback(GLFWwindow* window, int width, int height)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_width = width;
    win->m_height = height;
    glViewport(0, 0, width, height);
}