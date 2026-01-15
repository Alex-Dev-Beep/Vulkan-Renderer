#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdexcept>
#include <iostream>

#include "window.hpp"
#include "instance.hpp"
#include "surface.hpp"
#include "device.hpp"

int main() {
    try {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to init GLFW!");
        } else {
            std::cout << "Initialized GLFW correctly!" << std::endl;
        }

        GLFWwindow* window;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkInstance instance = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkDevice device = VK_NULL_HANDLE;

        createWindow(800, 600, "Vulkan Window", window);
        createInstance(instance);
        setupDebugMessenger(instance);
        createSurface(instance, surface, window);
        pickPhysicalDevice(instance, physicalDevice, surface);
        createLogicalDevice(physicalDevice, surface, device, graphicsQueue, presentQueue);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
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
