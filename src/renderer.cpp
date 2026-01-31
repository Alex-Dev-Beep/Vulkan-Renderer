#include "renderer.hpp"

renderer Renderer;

void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
    Renderer.framebufferResized = true;
}

void renderer::drawFrame() {
    vkWaitForFences(Device.device, 1, &Fences.inFlightFences[Renderer.currentFrame], VK_TRUE, UINT64_MAX);
    
    VkResult result = vkAcquireNextImageKHR(
        Device.device,
        SwapChain.swapChain,
        UINT64_MAX,
        Semaphores.imageAvailableSemaphores[Renderer.currentFrame],
        VK_NULL_HANDLE,
        &Renderer.imageIndex
    );
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        Fences.imagesInFlight.clear();
        Fences.imagesInFlight.resize(SwapChain.swapChainImages.size(), VK_NULL_HANDLE);
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }
    if (Fences.imagesInFlight[Renderer.imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(
            Device.device,
            1,
            &Fences.imagesInFlight[Renderer.imageIndex],
            VK_TRUE,
            UINT64_MAX
        );
    }
    vkResetCommandBuffer(
        Buffers.commandBuffers[Renderer.imageIndex],
        0
    );
    recordCommandBuffer(); 
    Fences.imagesInFlight[Renderer.imageIndex] = Fences.inFlightFences[Renderer.currentFrame];
    updateUniformBuffer(Renderer.currentFrame, UniformBuffers.uniformBuffersMapped, SwapChain.swapChainExtent);
    vkResetFences(Device.device, 1, &Fences.inFlightFences[Renderer.currentFrame]);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    VkSemaphore waitSemaphores[] = { Semaphores.imageAvailableSemaphores[Renderer.currentFrame] };
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &Buffers.commandBuffers[Renderer.imageIndex];
    VkSemaphore signalSemaphores[] = { Semaphores.renderFinishedSemaphores[Renderer.currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    if (vkQueueSubmit(
        Queues.graphicsQueue,
        1,
        &submitInfo,
        Fences.inFlightFences[Renderer.currentFrame]
    ) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &SwapChain.swapChain;
    presentInfo.pImageIndices = &Renderer.imageIndex;
    result = vkQueuePresentKHR(Queues.presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR ||
        result == VK_SUBOPTIMAL_KHR ||
        framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
        Fences.imagesInFlight.clear();
        Fences.imagesInFlight.resize(SwapChain.swapChainImages.size(), VK_NULL_HANDLE);
        return;
    }
    Renderer.currentFrame = (Renderer.currentFrame + 1) % Renderer.MAX_FRAMES_IN_FLIGHT;
}