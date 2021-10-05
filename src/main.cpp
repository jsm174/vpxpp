#include "VPApp.h"

#include <cxxopts.hpp>
#include <iostream>

#include "libplatform/libplatform.h"
#include "v8.h"

void v8_test()
{
	v8::V8::InitializeICUDefaultLocation(".");
	v8::V8::InitializeExternalStartupData(".");
	std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();

	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

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
			v8::Local<v8::String> source = v8::String::NewFromUtf8Literal(isolate, "'Hello' + ', World!'");

			// Compile the source code.
			v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

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

int main(int argc, char** argv)
{
	try
	{
		cxxopts::Options options("vpxpp", "Visual Pinball X Player +");

		options.add_options()("f,file", "VPX file", cxxopts::value<std::string>())(
			"disable-player", "Disable player", cxxopts::value<bool>()->default_value("false"))("h,help",
																								"Print usage");

		auto result = options.parse(argc, argv);

		if (result.count("help"))
		{
			std::cout << options.help() << std::endl;
			exit(0);
		}

		std::string filename;

		if (result.count("file"))
		{
			filename = result["file"].as<std::string>();
		}

		filename = "/Users/jmillard/git/vpxpp/tables/exampleTable.vpx";
		// filename = "C:\\tmp\\vpxpp\\tables\\exampleTable.vpx";
		// filename = "/Users/jmillard/Desktop/Pinball/Terminator 2 (Williams 1991).vpx";

		if (filename.empty())
		{
			throw cxxopts::OptionSpecException("Option 'f' is required");
		}

		VPApp vpApp;
		vpApp.InitInstance(filename);

		if (!result["disable-player"].as<bool>())
		{
			vpApp.Run();
		}

		v8_test();
	}

	catch (const cxxopts::OptionException& e)
	{
		std::cout << "Error parsing options: " << e.what() << std::endl;
		std::cout << "Try --help for usage information." << std::endl
				  << std::endl;
		exit(1);
	}

	return 0;
}
