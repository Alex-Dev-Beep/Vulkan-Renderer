#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "window.hpp"

int main() {
    Window window(800, 600, "Vulkan Window");

    while (!window.shouldClose()) {
        window.pollEvents();
    }

    return 0;
}
