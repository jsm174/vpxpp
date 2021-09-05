#include "EditableRegistry.h"
#include <map>
#include <stdio.h>

#include "Bumper.h"
#include "Decal.h"
#include "DispReel.h"
#include "Flasher.h"
#include "Flipper.h"
#include "Gate.h"
#include "HitTarget.h"
#include "Kicker.h"
#include "Light.h"
#include "LightSeq.h"
#include "PinTable.h"
#include "Plunger.h"
#include "Primitive.h"
#include "Ramp.h"
#include "Rubber.h"
#include "Spinner.h"
#include "Surface.h"
#include "Textbox.h"
#include "Timer.h"
#include "Trigger.h"

#include "libplatform/libplatform.h"
#include "v8.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

std::map<ItemTypeEnum, EditableInfo> EditableRegistry::m_map;

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
			printf("%s\n", *utf8);
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
		exit(EXIT_FAILURE);
	}

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

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
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwTerminate();
}

int main(int argc, char** argv)
{
	EditableRegistry::RegisterEditable<Bumper>();
	EditableRegistry::RegisterEditable<Decal>();
	EditableRegistry::RegisterEditable<DispReel>();
	EditableRegistry::RegisterEditable<Flasher>();
	EditableRegistry::RegisterEditable<Flipper>();
	EditableRegistry::RegisterEditable<Gate>();
	EditableRegistry::RegisterEditable<Kicker>();
	EditableRegistry::RegisterEditable<Light>();
	EditableRegistry::RegisterEditable<LightSeq>();
	EditableRegistry::RegisterEditable<Plunger>();
	EditableRegistry::RegisterEditable<Primitive>();
	EditableRegistry::RegisterEditable<Ramp>();
	EditableRegistry::RegisterEditable<Rubber>();
	EditableRegistry::RegisterEditable<Spinner>();
	EditableRegistry::RegisterEditable<Surface>();
	EditableRegistry::RegisterEditable<Textbox>();
	EditableRegistry::RegisterEditable<Timer>();
	EditableRegistry::RegisterEditable<Trigger>();
	EditableRegistry::RegisterEditable<HitTarget>();

	PinTable* pPinTable = new PinTable();

	std::string filename = "/Users/jmillard/git/vpxpp/tables/exampleTable.vpx";
	//std::string filename = "/Users/jmillard/git/vpxpp/tables/blankTable.vpx";
	//std::string filename = "/Users/jmillard/Desktop/Terminator 2 (Williams 1991).vpx";

	if (argc > 1)
	{
		filename = argv[1];
	}

	pPinTable->LoadGameFromFilename(filename.c_str());

	v8_test();

	glfw_test();

	printf("done");
}
