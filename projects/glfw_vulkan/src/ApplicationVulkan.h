#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "Component.h"
#include "Application.h"

class ApplicationVulkan
{
public:
	static int width, height;
	static char* app_name;

	GLFWwindow* m_windowHandle;
	bool m_running = true;

	static VkInstance vkInstance;
	static VkPhysicalDevice GetPhysicalDevice();
	static VkDevice GetDevice();
	static VkCommandBuffer GetCommandBuffer();
	static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

	ApplicationVulkan(int width, int height, char* app_name);
	void Init(int width, int height, char* description);
	int Run();
	void Close();
};
