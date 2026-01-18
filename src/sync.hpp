#define GLFW_INCLUDE_VULKAN
#include <GLFW//glfw3.h>

#include <vector>

void createSyncObjects(VkDevice device, VkFence& inFlightFence, VkSemaphore& renderFinishedSemaphore, VkSemaphore& imageAvailableSemaphore, std::vector<VkCommandBuffer> commandBuffers, std::vector<VkSemaphore> imageAvailableSemaphores, std::vector<VkSemaphore> renderFinishedSemaphores, std::vector<VkFence> inFlightFences, int MAX_FRAMES_IN_FLIGHT);