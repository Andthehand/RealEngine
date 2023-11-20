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


	////////////////////////////////
	///////////Constants////////////
	////////////////////////////////

	class ShaderConstantVec4Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec4Node();

		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = (glm::vec4&)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	private:
		glm::vec4 m_Constant = glm::vec4(1.0f);
	};

	class ShaderConstantVec3Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec3Node();

		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = (glm::vec3&)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	private:
		glm::vec3 m_Constant = glm::vec3(1.0f);
	};

	class ShaderConstantVec2Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec2Node();

		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = (glm::vec2&)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	private:
		glm::vec2 m_Constant = glm::vec2(1.0f);
	};

	class ShaderConstantFloatNode : public ShaderNodeConstant {
	public:
		ShaderConstantFloatNode();

		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = *(float*)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	private:
		float m_Constant = 1.0f;
	};
}
