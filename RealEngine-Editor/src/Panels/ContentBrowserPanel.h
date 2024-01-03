#pragma once
#include <filesystem>

#include "RealEngine/Renderer/Texture.h"

#include "VisualShader/ShaderPanelManager.h"

namespace RealEngine {
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;

		std::unordered_map<std::string, Ref<Texture2D>> m_ImageIcons;

		Ref<ShaderPanelManager> m_ShaderPanelManager;
	};
}
