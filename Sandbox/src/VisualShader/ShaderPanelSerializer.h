#pragma once
#include "ShaderPanel.h"

namespace RealEngine {
	class ShaderPanelSerializer {
	public:
		static void Serialize(Ref<ShaderPanel> shaderPanels[2], const std::filesystem::path& filepath);

		static void Deserialize(const std::filesystem::path& filepath, Ref<ShaderPanel>* shaderPanels);
	};
}
