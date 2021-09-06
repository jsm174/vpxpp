#include "VPApp.h"

#include <iostream>

#include "libplatform/libplatform.h"
#include "v8.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void v8_test()
{
	v8::V8::InitializeICUDefaultLocation(".");
	v8::V8::InitializeExternalStartupData(".");
	std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();

	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator =
	    v8::ArrayBuffer::Allocator::NewDefaultAllocator();

	v8::Isolate* isolate = v8::Isolate::New(create_params);
	{
		v8::Isolate::Scope isolate_scope(isolate);

		// Create a stack-allocated handle scope.
		v8::HandleScope handle_scope(isolate);

		// Create a new context.
		v8::Local<v8::Context> context = v8::Context::New(isolate);

		// Enter the context for compiling and running the hello world script.
		v8::Context::Scope context_scope(context);

		{
			// Create a string containing the JavaScript source code.
			v8::Local<v8::String> source =
			    v8::String::NewFromUtf8Literal(isolate, "'Hello' + ', World!'");

			// Compile the source code.
			v8::Local<v8::Script> script =
			    v8::Script::Compile(context, source).ToLocalChecked();

			// Run the script to get the result.
			v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

			// Convert the result to an UTF8 string and print it.
			v8::String::Utf8Value utf8(isolate, result);
			std::cout << *utf8 << std::endl;
		}
	}

	// Dispose the isolate and tear down V8.
	isolate->Dispose();
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void glfw_test()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "vpxpp", NULL, NULL);

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
	if (vkCreateInstance(&instanceCreateInfo, NULL, &instance) != VK_SUCCESS)
	{
		std::cout << "vkinstance failure" << std::endl;
		return;
	}

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	VkPhysicalDevice physicalDevice = devices[0];

	glfwSetKeyCallback(window, key_callback);

	int i = 0;

	while (!glfwWindowShouldClose(window) && i++ < 100000)
	{
		glfwPollEvents();
	}

	VkSurfaceKHR surface;
	VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);

	if (err)
	{
		std::cout << "surface failure" << std::endl;
	}

	glfwTerminate();
}

int main(int argc, char** argv)
{
	std::string filename = "/Users/jmillard/git/vpxpp/tables/exampleTable.vpx";
	//std::string filename = "/Users/jmillard/git/vpxpp/tables/blankTable.vpx";
	//std::string filename = "/Users/jmillard/Desktop/Terminator 2 (Williams 1991).vpx";

	if (argc > 1)
	{
		filename = argv[1];
	}

	VPApp vpApp;
	vpApp.InitInstance(filename);
	vpApp.Run();
	
	v8_test();
	glfw_test();

	std::cout << "done" << std::endl;
}
