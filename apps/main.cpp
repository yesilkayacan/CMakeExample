#include <iostream>

#include "Application.hpp"

int main(const int argc, const char *argv[])
{
    Application *app = new Application();

    app->Run();

    delete app;
    return 0;
}