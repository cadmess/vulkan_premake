#pragma once
#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

class ImGuiVulkanUtil
{
    // [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
    // To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
    // Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif
public:
    static inline VkAllocationCallbacks*   g_Allocator = nullptr;
    static inline VkInstance               g_Instance = VK_NULL_HANDLE;
    static inline VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
    static inline VkDevice                 g_Device = VK_NULL_HANDLE;
    static inline uint32_t                 g_QueueFamily = static_cast<uint32_t>(-1);
    static inline VkQueue                  g_Queue = VK_NULL_HANDLE;
    static inline VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
    static inline VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
    static inline VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

    static inline ImGui_ImplVulkanH_Window g_MainWindowData;
    static inline int                      g_MinImageCount = 2;
    static inline bool                     g_SwapChainRebuild = false;

    static void check_vk_result(VkResult err);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
#endif // IMGUI_VULKAN_DEBUG_REPORT

    static void SetupVulkan(const char** extensions, uint32_t extensions_count);

	// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
	// Your real engine/app may not use them.
	static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

	static void CleanupVulkan();

	static void CleanupVulkanWindow();

    static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);

    static void FramePresent(ImGui_ImplVulkanH_Window* wd);
};