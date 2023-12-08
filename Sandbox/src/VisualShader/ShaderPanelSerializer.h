#pragma once
#include "ShaderPanel.h"

namespace RealEngine {
	class ShaderPanelSerializer {
	public:
		static void Serialize(Ref<ShaderPanel> shaderPanel, const std::filesystem::path& filepath);

		static Ref<ShaderPanel>  Deserialize(const std::filesystem::path& filepath);
	};
}
