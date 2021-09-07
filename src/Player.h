#pragma once

#include "PinTable.h"
#include "Pin3D.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Player
{
public:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	Player(const bool cameraMode, PinTable* const ptable);
	virtual ~Player();

	bool m_cameraMode;
	bool m_hasWindow;

	PinTable* m_ptable;
    Pin3D m_pin3d;

	void Create();
	void Render();

	void RenderDynamics();
	
private:

	GLFWwindow* m_pWindow;
};