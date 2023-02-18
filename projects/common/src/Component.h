#pragma once
class Component
{
public:
	virtual ~Component() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}

	virtual void OnRender() {}
};