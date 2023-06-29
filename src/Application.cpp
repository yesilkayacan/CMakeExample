#include "Application.hpp"

Application::Application()
    : window(new Window(800, 600, "gui_app")),
      renderer(new eng::Renderer())
{
    renderer->Init();
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