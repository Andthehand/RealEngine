#pragma once
#include <glm/glm.hpp>

#include "ShaderNode.h"

namespace RealEngine {
	class FragmentShaderOutputNode : public ShaderNode {
	public:
		FragmentShaderOutputNode();

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
		std::string GenerateGlobalCode(std::string* inputVars) const override;
	};

	class ShaderTextureNode : public ShaderNode {
	public:
		ShaderTextureNode();

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	};

	class ShaderConstantVec4Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec4Node();

		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = (glm::vec4&)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	private:
		glm::vec4 m_Constant = glm::vec4(1.0f);
	};
}
