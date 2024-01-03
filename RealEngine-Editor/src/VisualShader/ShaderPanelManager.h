#pragma once
#include "ShaderPanel.h"

#include "RealEngine/Renderer/Shader.h"
#include "RealEngine/Renderer/Texture.h"
#include "RealEngine/Resources/Resource.h"

#include <filesystem>

namespace RealEngine {
	enum ShaderType : int {
		Vertex		= 0,
		Fragment	= 1,
	};

	class ShaderPanelManager : public Resource<ShaderPanelManager> {
	public:
		ShaderPanelManager(const std::filesystem::path& path);
		~ShaderPanelManager();

		void OnImGuiRender();

		std::string ShaderPanelManager::ReadFile(const std::filesystem::path& filepath);
		std::string PreProcess(std::filesystem::path& ubershader, CompileData shaderData[2]);
		void SaveAndCompile();
	private:
		ShaderType m_CurrentShaderType = ShaderType::Vertex;

		Ref<Texture2D> m_HeaderBackground;
		Ref<ShaderPanel> m_ShaderPanels[2];
	};
}
