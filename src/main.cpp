#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "window.hpp"
#include "instance.hpp"

int main() {
    VkInstance instance;
    GLFWwindow* window;

    createWindow(800, 600, "Vulkan Window", window);
    createInstance(instance);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
