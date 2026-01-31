#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdexcept>
#include <iostream>
#include <vector>

#include "window.hpp"
#include "instance.hpp"
#include "surface.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "sync.hpp"
#include "vertex.hpp"
#include "uniform.hpp"
#include "image.hpp"
#include "buffer.hpp"

#include <stdexcept>

struct renderer {
    public:
    bool framebufferResized = false;
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t imageIndex;
    uint32_t currentFrame = 0;
    void drawFrame();
};

void frameBufferResizeCallback(GLFWwindow* window, int width, int height);

extern renderer Renderer;