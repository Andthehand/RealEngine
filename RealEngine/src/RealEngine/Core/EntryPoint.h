#pragma once
#include "RealEngine/Core/Core.h"

#ifdef RE_PLATFORM_WINDOWS

//Looks for the decleration of this function in sandbox
extern RealEngine::Application* RealEngine::CreateApplication();

int main(int argc, char** argv) {
	RealEngine::Log::Init();

	RE_PROFILE_BEGIN_SESSION("Startup", "RealEngine-Startup.json");
	auto app = RealEngine::CreateApplication();
	RE_PROFILE_END_SESSION();

	RE_PROFILE_BEGIN_SESSION("Runtime", "RealEngine-Runtime.json");
	app->Run();
	RE_PROFILE_END_SESSION();

	RE_PROFILE_BEGIN_SESSION("Shutdown", "RealEngine-Shutdown.json");
	delete app;
	RE_PROFILE_END_SESSION();
}
#endif