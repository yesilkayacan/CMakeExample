#include "Window.hpp"
#include <iostream>

Window::Window(int width, int height, const char *title)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "GLFW initialized." << std::endl;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwSetErrorCallback(errorCallback);

    glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!glfwWindow)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Created GLFW window." << std::endl;
    }

    glfwSetKeyCallback(glfwWindow, keyCallback);
}

Window::~Window()
{
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
    std::cout << "Cleaned GLFW resources" << std::endl;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(glfwWindow);
}

void Window::pollEvents() const
{
    glfwPollEvents();
}

std::pair<const char **, uint32_t> Window::GetInstanceExtensions()
{
    uint32_t glfwExtensionCount;
    auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    return std::make_pair(glfwExtensions, glfwExtensionCount);
}

void Window::errorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // Handle key input events here, such as closing the window with the Escape key.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
