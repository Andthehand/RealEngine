#pragma once

#include "RealEngine/Core/Layer.h"

#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/KeyEvent.h"
#include "RealEngine/Events/MouseEvent.h"

namespace RealEngine {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
	};

}
