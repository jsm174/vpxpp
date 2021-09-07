#pragma once

#include "PinTable.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Player
{
public:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	Player(const bool cameraMode, PinTable* const ptable);
	virtual ~Player();

	bool m_cameraMode;
	PinTable* m_ptable;

	GLFWwindow* m_pWindow;

	void Create();
	void Render();

private:
};