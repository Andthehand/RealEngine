#pragma once

#include "Core.h"

#include "Window.h"
#include "RealEngine/Core/LayerStack.h"
#include "RealEngine/Events/Event.h"
#include "RealEngine/Events/ApplicationEvent.h"

#include "RealEngine/Core/TimeStep.h"

#include "RealEngine/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace RealEngine {
	struct ApplicationCommandLineArgs {
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const {
			RE_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class Application {
	public:
		Application(const std::string& name = "RealEngine App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application() = default;

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow() { return *m_Window; }
		
		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime;

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
