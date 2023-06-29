#pragma once

#include "Window.hpp"
#include "renderer/Renderer.hpp"

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    Window *window;
    eng::Renderer *renderer;
};