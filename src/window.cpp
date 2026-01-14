#include "window.hpp"

#include <stdexcept>
#include <iostream>

void createWindow(int width, int height, const char* title, GLFWwindow* window) {
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
