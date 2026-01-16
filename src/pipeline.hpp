#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void createGraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent, VkPipelineLayout& pipelineLayout, VkRenderPass renderPass, VkPipeline& graphicsPipeline);
void createRenderPass(VkFormat swapChainImageFormat, VkRenderPass& renderPass, VkDevice device);