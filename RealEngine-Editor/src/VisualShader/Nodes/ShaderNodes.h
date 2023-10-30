#pragma once

#include "ShaderNode.h"

namespace RealEngine {
	class FragmentShaderOutputNode : public ShaderNode {
	public:
		FragmentShaderOutputNode();

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) override;
	};

	class ShaderTextureNode : public ShaderNode {
	public:
		ShaderTextureNode();

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) override;
	};
}
