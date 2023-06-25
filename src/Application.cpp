#include "Application.hpp"

Application::Application()
    : window(new Window(800, 600, "gui_app"))
{
}

Application::~Application() {}

void Application::Run()
{
    while (!window->shouldClose())
    {
        // Update and render the UML diagram using the renderer.

        window->pollEvents();
    }
}