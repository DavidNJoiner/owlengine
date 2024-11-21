#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "core/ISystem.h"
#include "core/Keycodes.h"
#include "renderer/Window.h"

#define MAX_KEYS (GLFW_KEY_LAST + 1)
#define MAX_BUTTONS 32

class InputManager : public ISystem {
public:
    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }

    // Delete copy and move constructors and assign operators
    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    void Update(float deltaTime) override;

    bool IsKeyPressed(unsigned int keycode) const;
    bool IsKeyReleased(unsigned int keycode) const;
    bool IsKeyHeld(unsigned int keycode) const;
    bool IsMouseButtonPressed(unsigned int button) const;
    bool IsMouseButtonReleased(unsigned int button) const;
    bool IsMouseButtonHeld(unsigned int button) const;
    void GetMousePosition(double& x, double& y) const;
    void GetMouseDelta(float& dx, float& dy) const;

    void SetWindow(GLFWwindow* window);

private:
    GLFWwindow* m_Window;
    bool m_Keys[MAX_KEYS];
    bool m_KeysChanged[MAX_KEYS];
    bool m_MouseButtons[MAX_BUTTONS];
    bool m_MouseButtonsChanged[MAX_BUTTONS];
    double m_MouseX, m_MouseY;
    float m_LastMouseX, m_LastMouseY;
    float m_DeltaX, m_DeltaY;
    bool m_FirstMouse;

private:
    InputManager(); // Private constructor
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif // INPUT_MANAGER_H