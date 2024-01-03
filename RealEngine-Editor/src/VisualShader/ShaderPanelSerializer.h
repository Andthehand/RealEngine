#pragma once
#include "ShaderPanel.h"

namespace RealEngine {
	class ShaderPanelSerializer {
	public:
		static void Serialize(Ref<ShaderPanel> shaderPanels[2], const std::filesystem::path& filepath, const std::string& shaderCode);

		static bool Deserialize(const std::filesystem::path& filepath, Ref<ShaderPanel>* shaderPanels);
	};
}
