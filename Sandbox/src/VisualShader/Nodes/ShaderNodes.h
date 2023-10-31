#pragma once

#include "ShaderNode.h"

namespace RealEngine {
	class FragmentShaderOutputNode : public ShaderNode {
	public:
		FragmentShaderOutputNode();

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	};

	class ShaderTextureNode : public ShaderNode {
	public:
		ShaderTextureNode();

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	};

	class ShaderConstantVec4Node : public ShaderNode {
	public:
		ShaderConstantVec4Node();

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	};
}
