#pragma once
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <cstdio>          // printf, fprintf
#include "ApplicationVulkan.h"
#include "ImGuiVulkanUtil.h"
#include <GLFW/glfw3.h>



int ApplicationVulkan::Init(int width, int height, char* description)
{
    // Setup GLFW window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_windowHandle = glfwCreateWindow(width, height, description, nullptr, nullptr);

    // Setup Vulkan
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
        return 1;
    }
    uint32_t extensions_count = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
    ImGuiVulkanUtil::SetupVulkan(extensions, extensions_count);

    // Create Window Surface
    VkSurfaceKHR surface;
    VkResult err = glfwCreateWindowSurface(ImGuiVulkanUtil::g_Instance, m_windowHandle, ImGuiVulkanUtil::g_Allocator, &surface);
    ImGuiVulkanUtil::check_vk_result(err);

    // Create Framebuffers
    int w, h;
    glfwGetFramebufferSize(m_windowHandle, &w, &h);
    ImGui_ImplVulkanH_Window* wd = &ImGuiVulkanUtil::g_MainWindowData;
    ImGuiVulkanUtil::SetupVulkanWindow(wd, surface, w, h);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer ImGuiVulkanUtils
    ImGui_ImplGlfw_InitForVulkan(m_windowHandle, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = ImGuiVulkanUtil::g_Instance;
    init_info.PhysicalDevice = ImGuiVulkanUtil::g_PhysicalDevice;
    init_info.Device = ImGuiVulkanUtil::g_Device;
    init_info.QueueFamily = ImGuiVulkanUtil::g_QueueFamily;
    init_info.Queue = ImGuiVulkanUtil::g_Queue;
    init_info.PipelineCache = ImGuiVulkanUtil::g_PipelineCache;
    init_info.DescriptorPool = ImGuiVulkanUtil::g_DescriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = ImGuiVulkanUtil::g_MinImageCount;
    init_info.ImageCount = wd->ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = ImGuiVulkanUtil::g_Allocator;
    init_info.CheckVkResultFn = ImGuiVulkanUtil::check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Upload Fonts
    {
        // Use any command queue
        VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
        VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

        err = vkResetCommandPool(ImGuiVulkanUtil::g_Device, command_pool, 0);
        ImGuiVulkanUtil::check_vk_result(err);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(command_buffer, &begin_info);
        ImGuiVulkanUtil::check_vk_result(err);

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        err = vkEndCommandBuffer(command_buffer);
        ImGuiVulkanUtil::check_vk_result(err);
        err = vkQueueSubmit(ImGuiVulkanUtil::g_Queue, 1, &end_info, VK_NULL_HANDLE);
        ImGuiVulkanUtil::check_vk_result(err);

        err = vkDeviceWaitIdle(ImGuiVulkanUtil::g_Device);
        ImGuiVulkanUtil::check_vk_result(err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
    return 0;
}

int ApplicationVulkan::Run()
{
    ImGui_ImplVulkanH_Window* wd = &ImGuiVulkanUtil::g_MainWindowData;
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    constexpr ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Main loop
    while (!glfwWindowShouldClose(m_windowHandle) && m_running)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Resize swap chain?
        if (ImGuiVulkanUtil::g_SwapChainRebuild)
        {
            int width, height;
            glfwGetFramebufferSize(m_windowHandle, &width, &height);
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(ImGuiVulkanUtil::g_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(ImGuiVulkanUtil::g_Instance, ImGuiVulkanUtil::g_PhysicalDevice, ImGuiVulkanUtil::g_Device, &ImGuiVulkanUtil::g_MainWindowData, ImGuiVulkanUtil::g_QueueFamily, ImGuiVulkanUtil::g_Allocator, width, height, ImGuiVulkanUtil::g_MinImageCount);
                ImGuiVulkanUtil::g_MainWindowData.FrameIndex = 0;
                ImGuiVulkanUtil::g_SwapChainRebuild = false;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
            if (m_menuBarCallback)
                window_flags |= ImGuiWindowFlags_MenuBar;

            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", nullptr, window_flags);
            ImGui::PopStyleVar();

            ImGui::PopStyleVar(2);
            // Submit the DockSpace
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                const ImGuiID dockspace_id = ImGui::GetID("VulkanAppDockspace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (m_menuBarCallback)
            {
                if (ImGui::BeginMenuBar())
                {
                    m_menuBarCallback();
                    ImGui::EndMenuBar();
                }
            }

            for (const auto& layer : m_ComponentStack)
                layer->OnRender();

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImDrawData* main_draw_data = ImGui::GetDrawData();
        const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
        wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
        wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
        wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
        wd->ClearValue.color.float32[3] = clear_color.w;
        if (!main_is_minimized)
            ImGuiVulkanUtil::FrameRender(wd, main_draw_data);

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        // Present Main Platform Window
        if (!main_is_minimized)
            ImGuiVulkanUtil::FramePresent(wd);
    }

    // Cleanup
    ImGuiVulkanUtil::check_vk_result(vkDeviceWaitIdle(ImGuiVulkanUtil::g_Device));
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ImGuiVulkanUtil::CleanupVulkanWindow();
    ImGuiVulkanUtil::CleanupVulkan();

    glfwDestroyWindow(this->m_windowHandle);
    glfwTerminate();

    this->Close();
    return 0;
}

void ApplicationVulkan::Close()
{
    m_running = false;
}

VkInstance ApplicationVulkan::GetInstance()
{
    return ImGuiVulkanUtil::g_Instance;
}

VkPhysicalDevice ApplicationVulkan::GetPhysicalDevice()
{
    return ImGuiVulkanUtil::g_PhysicalDevice;
}

VkDevice ApplicationVulkan::GetDevice()
{
    return ImGuiVulkanUtil::g_Device;
}

VkCommandBuffer ApplicationVulkan::GetCommandBuffer()
{
    const ImGui_ImplVulkanH_Window* wd = &ImGuiVulkanUtil::g_MainWindowData;

    VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
    cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufAllocateInfo.commandPool = wd->Frames[wd->FrameIndex].CommandPool;
    cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufAllocateInfo.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    auto err = vkAllocateCommandBuffers(ImGuiVulkanUtil::g_Device, &cmdBufAllocateInfo, &command_buffer);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(command_buffer, &begin_info);
    ImGuiVulkanUtil::check_vk_result(err);

    return command_buffer;
}

void ApplicationVulkan::FlushCommandBuffer(VkCommandBuffer commandBuffer)
{
    constexpr uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

    VkSubmitInfo end_info = {};
    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    end_info.commandBufferCount = 1;
    end_info.pCommandBuffers = &commandBuffer;
    auto err = vkEndCommandBuffer(commandBuffer);
    ImGuiVulkanUtil::check_vk_result(err);

    // Create fence to ensure that the command buffer has finished executing
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = 0;
    VkFence fence;
    err = vkCreateFence(ImGuiVulkanUtil::g_Device, &fenceCreateInfo, nullptr, &fence);
    ImGuiVulkanUtil::check_vk_result(err);

    err = vkQueueSubmit(ImGuiVulkanUtil::g_Queue, 1, &end_info, fence);
    ImGuiVulkanUtil::check_vk_result(err);

    err = vkWaitForFences(ImGuiVulkanUtil::g_Device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
    ImGuiVulkanUtil::check_vk_result(err);

    vkDestroyFence(ImGuiVulkanUtil::g_Device, fence, nullptr);
}