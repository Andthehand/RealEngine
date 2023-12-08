#pragma once
#include "ShaderPanel.h"

#include "RealEngine/Renderer/Shader.h"
#include "RealEngine/Renderer/Texture.h"

namespace RealEngine {
	enum ShaderType  : int {
		Vertex		= 0,
		Fragment	= 1,
	};

	class ShaderPanelManager {
	public:
		ShaderPanelManager();
		~ShaderPanelManager();

		void OnImGuiRender();
		void OnUpdate();
		void Compile();
		
		Ref<Shader> GetShader() { return m_PreviewShader; }
	private:
		ShaderType m_CurrentShaderType = ShaderType::Vertex;

		Ref<Texture2D> m_HeaderBackground;
		Ref<ShaderPanel> m_ShaderPanels[2];

		bool m_QueuedCompile = true;
		Ref<Shader> m_PreviewShader;
		ShaderReflect m_Reflect;
	};
}
