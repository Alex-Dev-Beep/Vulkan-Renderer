#include "sync.hpp"

#include <stdexcept>
#include <iostream>

void createSyncObjects(VkDevice device, VkFence& inFlightFence, VkSemaphore& renderFinishedSemaphore, VkSemaphore& imageAvailableSemaphore, std::vector<VkCommandBuffer> commandBuffers, std::vector<VkSemaphore> imageAvailableSemaphores, std::vector<VkSemaphore> renderFinishedSemaphores, std::vector<VkFence> inFlightFences, int MAX_FRAMES_IN_FLIGHT) {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create semaphores!");
    }

    std::cout << "Succesfully created semaphores!" << std::endl;
}
