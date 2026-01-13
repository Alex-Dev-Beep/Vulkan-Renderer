#include "window.hpp"

#include <stdexcept>
#include <iostream>

Window::Window(int width, int height, const char* title) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to init GLFW!");
    } else {
        std::cout << "Initialized GLFW correctly!" << std::endl;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    } else {
        std::cout << "Created GLFW window!" << std::endl;
    }
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}
