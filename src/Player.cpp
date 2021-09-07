#include "Player.h"

#include <iostream>

void Player::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

Player::Player(const bool cameraMode, PinTable* const pPinTable)
{
	m_cameraMode = cameraMode;
	m_ptable = pPinTable;

	glfwInit();
}

Player::~Player()
{
	glfwTerminate();
}

void Player::Create()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_pWindow = glfwCreateWindow(1024, 768, "vpxpp", NULL, NULL);

	if (m_pWindow)
	{
		glfwSetKeyCallback(m_pWindow, Player::KeyCallback);

		uint32_t count;
		const char** extensions = glfwGetRequiredInstanceExtensions(&count);

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = count;
		instanceCreateInfo.ppEnabledExtensionNames = extensions;

		VkInstance instance;
		VkResult err = vkCreateInstance(&instanceCreateInfo, NULL, &instance);

		if (err != VK_SUCCESS)
		{
			std::cout << "vkCreateInstance failure: " << err << std::endl;

			glfwDestroyWindow(m_pWindow);
			m_pWindow = NULL;
		}
		else
		{
			VkSurfaceKHR surface;
			err = glfwCreateWindowSurface(instance, m_pWindow, NULL, &surface);

			if (err != VK_SUCCESS)
			{
				std::cout << "glfwCreateWindowSurface failure: " << err << std::endl;

				glfwDestroyWindow(m_pWindow);
				m_pWindow = NULL;
			}
		}
	}
}

void Player::Render()
{
	if (!m_pWindow)
	{
		return;
	}

	if (glfwWindowShouldClose(m_pWindow))
	{
		glfwDestroyWindow(m_pWindow);
		m_pWindow = NULL;

		return;
	}

	glfwPollEvents();
}