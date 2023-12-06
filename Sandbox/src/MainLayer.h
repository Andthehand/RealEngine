#pragma once
#include <RealEngine.h>

#include "VisualShader/ShaderPanelManager.h"

namespace RealEngine {
	class MainLayer : public Layer {
	public:
		MainLayer();
		
		virtual void OnAttach() override;
		virtual void OnImGuiRender() override;

		virtual void OnUpdate(Timestep ts) override;
	private:
		ShaderPanelManager m_ShaderPanelManager;

		Ref<Framebuffer> m_Framebuffer;

		Ref<VertexArray> m_VertexArray;

		Ref<Texture2D> m_Texture;
	};
}
