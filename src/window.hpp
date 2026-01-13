#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    bool shouldClose();
    void pollEvents();

private:
    GLFWwindow* window;
};
