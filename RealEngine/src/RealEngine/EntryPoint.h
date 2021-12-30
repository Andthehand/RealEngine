#pragma once

#ifdef RE_PLATFORM_WINDOWS

//Looks for the decleration of this function in sandbox
extern RealEngine::Application* RealEngine::CreateApplication();

int main(int argc, char** argv) {
	RealEngine::Log::Init();
	RE_CORE_WARN("Initialized Log!");

	auto app = RealEngine::CreateApplication();
	app->Run();
	delete app;
}
#endif