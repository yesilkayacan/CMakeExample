#pragma once

#include "Window.hpp"

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    Window *window;
};