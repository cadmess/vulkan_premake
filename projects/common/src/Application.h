#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <GLFW/glfw3.h>

#include "Component.h"

class Application
{
public:
	std::vector<std::shared_ptr<Component>> m_ComponentStack;
	GLFWwindow* m_windowHandle;
	bool m_running = true;
	std::function<void()> m_menuBarCallback;

	template<typename T>
	void PushComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "Pushed type is not a Component!");
		m_ComponentStack.emplace_back(std::make_shared<T>())->OnAttach();
	}

	void PushComponent(const std::shared_ptr<Component>& component)
	{
		m_ComponentStack.emplace_back(component); component->OnAttach();
	}

	static void glfw_error_callback(const int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}

	virtual ~Application() {}

    virtual int Init(int width, int height, char* description) = 0;

	virtual int Run() = 0;

	virtual void Close() = 0;
};