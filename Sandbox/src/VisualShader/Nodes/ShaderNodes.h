#pragma once
#include <glm/glm.hpp>

#include "ShaderNode.h"

namespace RealEngine {
	class FragmentShaderOutputNode : public ShaderNode {
	public:
		FragmentShaderOutputNode();

		const char* GetName() const override { return s_Name; }


		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
		std::string GenerateGlobalCode(std::string* inputVars) const override;

	public:
		inline static const char* s_Name = "Fragment Output";
	};

	////////////////////////////////
	///////////Textures/////////////
	////////////////////////////////

	class ShaderTextureNode : public ShaderNode {
	public:
		ShaderTextureNode();

		const char* GetName() const override { return s_Name; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
		std::string GenerateGlobalCode(std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Texture2D";
	};


	////////////////////////////////
	///////////Constants////////////
	////////////////////////////////

	class ShaderConstantVec4Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec4Node();

		const char* GetName() const override { return s_Name; }
		
		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = (glm::vec4&)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Vector4";
		
	private:
		glm::vec4 m_Constant = glm::vec4(1.0f);
	};

	class ShaderConstantVec3Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec3Node();

		const char* GetName() const override { return s_Name; }
		
		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = (glm::vec3&)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Vector3";
		
	private:
		glm::vec3 m_Constant = glm::vec3(1.0f);
	};

	class ShaderConstantVec2Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec2Node();

		const char* GetName() const override { return s_Name; }

		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = (glm::vec2&)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Vector2";
		
	private:
		glm::vec2 m_Constant = glm::vec2(1.0f);
	};

	class ShaderConstantFloatNode : public ShaderNodeConstant {
	public:
		ShaderConstantFloatNode();

		const char* GetName() const override { return s_Name; }

		void* GetConstant() const { return (void*)&m_Constant; }
		void SetConstant(void* constant) { m_Constant = *(float*)constant; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Float";
		
	private:
		float m_Constant = 1.0f;
	};
}
