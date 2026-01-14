#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdexcept>
#include <iostream>

#include "window.hpp"
#include "instance.hpp"

int main() {
    try {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to init GLFW!");
        } else {
            std::cout << "Initialized GLFW correctly!" << std::endl;
        }

        GLFWwindow* window;
        createWindow(800, 600, "Vulkan Window", window);

        VkInstance instance;
        createInstance(instance);
        setupDebugMessenger(instance);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

        cleanupInstance(instance);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
