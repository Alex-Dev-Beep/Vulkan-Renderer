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

uint32_t currentFrame = 0;

void drawFrame(VkDevice device, VkFence inFlightFence, VkSwapchainKHR swapChain, VkSemaphore imageAvailableSemaphore, VkCommandBuffer commandBuffer, VkRenderPass renderPass, std::vector<VkFramebuffer> swapChainFramebuffers, VkExtent2D swapChainExtent, VkPipeline graphicsPipeline, VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderFinishedSemaphore, std::vector<VkFence> inFlightFences, std::vector<VkSemaphore> renderFinishedSemaphores, std::vector<VkSemaphore> imageAvailableSemaphores, std::vector<VkCommandBuffer> commandBuffers, int MAX_FRAMES_IN_FLIGHT) {
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(commandBuffers[currentFrame],  0);

    recordCommandBuffer(commandBuffers[currentFrame], imageIndex, renderPass, swapChainFramebuffers, swapChainExtent, graphicsPipeline);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    std::cout << "Succesfully submitted drawed command buffer!" << std::endl;

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(presentQueue, &presentInfo);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

}

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
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;
        VkPipeline graphicsPipeline;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        const int MAX_FRAMES_IN_FLIGHT = 2;

        createWindow(800, 600, "Vulkan Window", window);
        createInstance(instance);
        setupDebugMessenger(instance);
        createSurface(instance, surface, window);
        pickPhysicalDevice(instance, physicalDevice, surface);
        createLogicalDevice(physicalDevice, surface, device, graphicsQueue, presentQueue);
        createSwapChain(physicalDevice, surface, window, swapChain, device, swapChainImages, swapChainImageFormat, swapChainExtent);
        createImageViews(swapChainImageViews, swapChainImages, swapChainImageFormat, device);
        createRenderPass(swapChainImageFormat, renderPass, device);
        createGraphicsPipeline(device, swapChainExtent, pipelineLayout, renderPass, graphicsPipeline);
        createFramebuffers(swapChainFramebuffers, swapChainImageViews, renderPass, swapChainExtent, device);
        createCommandPool(physicalDevice, surface, commandPool, device);
        createCommandBuffer(commandPool, commandBuffer, device, MAX_FRAMES_IN_FLIGHT, commandBuffers);
        createSyncObjects(device, inFlightFence, renderFinishedSemaphore, imageAvailableSemaphore, commandBuffers, imageAvailableSemaphores, renderFinishedSemaphores, inFlightFences, MAX_FRAMES_IN_FLIGHT);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            drawFrame(device, inFlightFence, swapChain, imageAvailableSemaphore, commandBuffer, renderPass, swapChainFramebuffers, swapChainExtent, graphicsPipeline, graphicsQueue, presentQueue, renderFinishedSemaphore, inFlightFences, renderFinishedSemaphores, imageAvailableSemaphores, commandBuffers, MAX_FRAMES_IN_FLIGHT);
        }
        
        vkDeviceWaitIdle(device);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }
        vkDestroyCommandPool(device, commandPool, nullptr);
        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }
        for (auto framebuffer : swapChainFramebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        vkDestroyRenderPass(device, renderPass, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroySwapchainKHR(device, swapChain, nullptr);
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
