#pragma once
#include "ApplicationVulkan.h"
#include "Component.h"
#include "imgui.h"


class TestComponent final : public Component
{
public:
    void OnRender() override
    {
        ImGui::ShowDemoWindow();
    }
};

int main(int, char**)
{
    Application* app = new ApplicationVulkan(800, 600, "test");

    VkResult vkResult = app->Init(1280, 720, "Hello");

    app->PushComponent<TestComponent>();

    app->Run();
}