#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdexcept>
#include <iostream>

#include "window.hpp"
#include "instance.hpp"
#include "surface.hpp"

int main() {
    try {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to init GLFW!");
        } else {
            std::cout << "Initialized GLFW correctly!" << std::endl;
        }

        GLFWwindow* window;
        VkSurfaceKHR surface;
        VkInstance instance;

        createWindow(800, 600, "Vulkan Window", window);
        createInstance(instance);
        setupDebugMessenger(instance);
        createSurface(instance, surface, window);

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
