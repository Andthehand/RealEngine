#pragma once

#include "RealEngine/Layer.h"

#include "RealEngine/Events/KeyEvent.h"
#include "RealEngine/Events/MouseEvent.h"
#include "RealEngine/Events/ApplicationEvent.h"

namespace RealEngine {
	class  ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}