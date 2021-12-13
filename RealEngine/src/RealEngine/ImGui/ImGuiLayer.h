#pragma once

#include "RealEngine/Layer.h"

#include "RealEngine/Events/KeyEvent.h"
#include "RealEngine/Events/MouseEvent.h"
#include "RealEngine/Events/ApplicationEvent.h"

namespace RealEngine {
	class RealEngine_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		
		bool OnWindowResizedEvent(WindowResizeEvent& e);


		float m_Time = 0.0f;
	};
}