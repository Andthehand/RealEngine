#pragma once

#ifdef RE_PLATFORM_WINDOWS

extern RealEngine::Application* RealEngine::CreateApplication();
int main(int argc, char** argv) {
	auto app = RealEngine::CreateApplication();
	app->Run();
	delete app;
}
#endif