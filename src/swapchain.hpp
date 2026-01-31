#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vertex.hpp"

#include <vector>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct swapChain {
    public:
    VkSwapchainKHR swapChain;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;

    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

void createSwapChain();

void createImageViews();
void cleanupSwapChain(VkDevice& device, VkRenderPass& renderPass, std::vector<VkFramebuffer>& swapChainFramebuffers, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, std::vector<VkImageView>& swapChainImageViews, VkSwapchainKHR& swapChain, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout);
void recreateSwapChain();

extern swapChain SwapChain;