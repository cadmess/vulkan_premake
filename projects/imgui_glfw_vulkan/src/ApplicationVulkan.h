#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "Component.h"
#include "Application.h"

class ApplicationVulkan : public Application
{
public:
	static VkInstance GetInstance();
	static VkPhysicalDevice GetPhysicalDevice();
	static VkDevice GetDevice();
	static VkCommandBuffer GetCommandBuffer();
	static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

	int Init(int width, int height, char* description) override;
	int Run() override;
	void Close() override;
};
