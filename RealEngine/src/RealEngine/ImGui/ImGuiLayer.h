#pragma once

#include "RealEngine/Core/Layer.h"

#include "RealEngine/Events/KeyEvent.h"
#include "RealEngine/Events/MouseEvent.h"
#include "RealEngine/Events/ApplicationEvent.h"

namespace RealEngine {
	class  ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}