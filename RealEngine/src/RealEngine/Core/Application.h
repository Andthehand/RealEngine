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

	class Application {
	public:
		Application();
		virtual ~Application() = default;

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		
		void Close();

		inline static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime;

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};


	Application* CreateApplication();
}
