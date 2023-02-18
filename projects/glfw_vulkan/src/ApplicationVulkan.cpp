#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <cstdio>          // printf, fprintf
#include "ApplicationVulkan.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

ApplicationVulkan::ApplicationVulkan(int width, int height, char* app_name) {

}

void ApplicationVulkan::Init(int width, int height, char* description)
{
    //check vulkan support
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
        return;
    }

    //set up glfw window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_windowHandle = glfwCreateWindow(width, height, description, nullptr, nullptr);

    //set up vulkan
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = app_name;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "None";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    //set up glfw extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create vulkan instance: " + result);
    }
}

int ApplicationVulkan::Run()
{
    while (!glfwWindowShouldClose(m_windowHandle) && m_running)
    {
        //poll and render crap
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(this->m_windowHandle);
    glfwTerminate();

    this->Close();
    return 0;
}

void ApplicationVulkan::Close()
{
    m_running = false;
}
static void glfw_error_callback(const int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}