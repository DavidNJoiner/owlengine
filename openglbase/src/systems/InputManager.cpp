#include "systems/InputManager.h"
#include <iostream>

InputManager::InputManager()
    : m_Window(nullptr), m_MouseX(0), m_MouseY(0), m_LastMouseX(0), m_LastMouseY(0),
    m_DeltaX(0), m_DeltaY(0), m_FirstMouse(true)
{
    for (bool& key : m_Keys) key = false;
    for (bool& key : m_KeysChanged) key = false;
    for (bool& button : m_MouseButtons) button = false;
    for (bool& button : m_MouseButtonsChanged) button = false;
}

void InputManager::Update(float deltaTime)
{
    // Reset key changed states
    for (int i = 0; i < MAX_KEYS; ++i) {
        m_KeysChanged[i] = false;
    }

    // Update keys within the valid range only
    for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i) {
        int state = glfwGetKey(m_Window, i);
        bool isPressed = (state == GLFW_PRESS || state == GLFW_REPEAT);
        if (isPressed != m_Keys[i]) {
            m_KeysChanged[i] = true;
        }
        m_Keys[i] = isPressed;
    }

    // Reset mouse button changed states
    for (int i = 0; i < MAX_BUTTONS; ++i) {
        m_MouseButtonsChanged[i] = false;
    }

    // Reset mouse delta
    m_DeltaX = 0.0f;
    m_DeltaY = 0.0f;

    // Poll for window events
    glfwPollEvents();
}



bool InputManager::IsKeyPressed(unsigned int keycode) const
{
    return (m_Keys[keycode] && m_KeysChanged[keycode]);
}

bool InputManager::IsKeyReleased(unsigned int keycode) const
{
    return !m_Keys[keycode] && m_KeysChanged[keycode];
}

bool InputManager::IsKeyHeld(unsigned int keycode) const
{
    return (m_Keys[keycode]);
}

bool InputManager::IsMouseButtonPressed(unsigned int button) const
{
    return m_MouseButtons[button] && m_MouseButtonsChanged[button];
}

bool InputManager::IsMouseButtonReleased(unsigned int button) const
{
    return !m_MouseButtons[button] && m_MouseButtonsChanged[button];
}

bool InputManager::IsMouseButtonHeld(unsigned int button) const
{
    return m_MouseButtons[button];
}

void InputManager::GetMousePosition(double& x, double& y) const
{
    x = m_MouseX;
    y = m_MouseY;
}

void InputManager::GetMouseDelta(float& dx, float& dy) const
{
    dx = m_DeltaX;
    dy = m_DeltaY;
}

void InputManager::SetWindow(GLFWwindow* window)
{
    m_Window = window;
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, MousePositionCallback);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    inputManager->m_Keys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    inputManager->m_KeysChanged[key] = (action == GLFW_PRESS);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    inputManager->m_MouseButtons[button] = action != GLFW_RELEASE;
    inputManager->m_MouseButtonsChanged[button] = true;
}

void InputManager::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager->m_FirstMouse)
    {
        inputManager->m_LastMouseX = xpos;
        inputManager->m_LastMouseY = ypos;
        inputManager->m_FirstMouse = false;
    }

    inputManager->m_DeltaX = xpos - inputManager->m_LastMouseX;
    inputManager->m_DeltaY = inputManager->m_LastMouseY - ypos;
    inputManager->m_LastMouseX = xpos;
    inputManager->m_LastMouseY = ypos;
    inputManager->m_MouseX = xpos;
    inputManager->m_MouseY = ypos;
}