#pragma once
#include "RealEngine/Core/Core.h"
#include "RealEngine/Core/Application.h"

#ifdef RE_PLATFORM_WINDOWS

//Looks for the decleration of this function in sandbox
extern RealEngine::Application* RealEngine::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) {
	RealEngine::Log::Init();
	
	RE_PROFILE_BEGIN_SESSION();
	auto app = RealEngine::CreateApplication({ argc, argv });

	app->Run();

	delete app;
	RE_PROFILE_END_SESSION();
	RE_PROFILE_SAVE_SESSION("logs/RealEngine.opt");

	RE_PROFILE_SHUTDOWN();
}
#endif
