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
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
        const int MAX_FRAMES_IN_FLIGHT = 2;
        uint32_t imageIndex;
        uint32_t currentFrame = 0;
        void drawFrame();
    };

void frameBufferResizeCallback(GLFWwindow* window, int width, int height);

extern renderer Renderer;