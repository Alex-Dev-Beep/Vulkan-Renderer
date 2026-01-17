#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

void createGraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent, VkPipelineLayout& pipelineLayout, VkRenderPass renderPass, VkPipeline& graphicsPipeline);
void createRenderPass(VkFormat swapChainImageFormat, VkRenderPass& renderPass, VkDevice device);
void createFramebuffers(std::vector<VkFramebuffer>& swapChainFramebuffers, std::vector<VkImageView>& swapChainImageViews, VkRenderPass renderPass, VkExtent2D swapChainExtent, VkDevice device);