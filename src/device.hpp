#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void pickPhysicalDevice(VkInstance instance, VkPhysicalDevice& physicalDevice);
bool isDeviceSuitable(VkPhysicalDevice device);