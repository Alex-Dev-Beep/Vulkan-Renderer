#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

struct pipeline {
    public:
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descriptorSetLayout;
};

void createGraphicsPipeline();
void createRenderPass();
void createFramebuffers();

extern pipeline Pipeline;