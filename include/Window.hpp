#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char *title);
    ~Window();
    bool shouldClose() const;
    void pollEvents() const;

    GLFWwindow *getGLFWwindow() const { return glfwWindow; }

private:
    static void errorCallback(int error, const char *description);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    GLFWwindow *glfwWindow;
};
