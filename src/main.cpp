#include "VPApp.h"

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

	std::cout << "done" << std::endl;
}
