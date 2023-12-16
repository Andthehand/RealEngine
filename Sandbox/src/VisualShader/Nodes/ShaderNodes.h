#pragma once
#include <glm/glm.hpp>

#include "ShaderNode.h"

namespace RealEngine {
	class VertexShaderOutputNode : public ShaderNode {
	public:
		VertexShaderOutputNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return ""; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;

	public:
		inline static const char* s_Name = "Fragment Output";
	};

	class FragmentShaderOutputNode : public ShaderNode {
	public:
		FragmentShaderOutputNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return ""; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;

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
		const char* GetOptionPath() const override { return s_OptionPath; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
		std::string GenerateGlobalCode(std::string* inputVars, std::vector<std::string>* defines) const override;
	public:
		inline static const char* s_Name = "Texture2D";
		inline static const char* s_OptionPath = "Textures";
	};

	////////////////////////////////
	///////////Inputs///////////////
	////////////////////////////////

	class ShaderInputNode : public ShaderNode {
	public:
		ShaderInputNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		const std::vector<const char*>& GetVariantOptions(int index) const override { return m_InputTypes[index]; };
		const std::vector<int>& GetVariantOptionsIndex() const override { return m_InputTypesIndex; };
		void SetVariantOptionsIndex(int vectorIndex, int stringIndex) override { m_InputTypesIndex[vectorIndex] = stringIndex; };

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
		std::string GenerateGlobalCode(std::string* inputVars, std::vector<std::string>* defines) const override;
	public:
		inline static const char* s_Name = "Input";
		inline static const char* s_OptionPath = "Inputs";

	private:
		std::vector<int> m_InputTypesIndex = { 0 };

		static inline std::array<std::vector<const char*>, 1> m_InputTypes = {
			{
				{"Position", "UV"}
			}
		};
	};

	////////////////////////////////
	///////////Vectors//////////////
	////////////////////////////////

	class ShaderVectorComposeNode : public ShaderNode {
	public:
		ShaderVectorComposeNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		const std::vector<const char*>& GetVariantOptions(int index) const override { return m_VectorTypes[index]; };
		const std::vector<int>& GetVariantOptionsIndex() const override { return m_VectorTypesIndex; };
		void SetVariantOptionsIndex(int vectorIndex, int stringIndex) override { m_VectorTypesIndex[vectorIndex] = stringIndex; };

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Vector Compose";
		inline static const char* s_OptionPath = "Vectors/Composition";
	private:
		std::vector<int> m_VectorTypesIndex = { 0 };

		static inline std::array<std::vector<const char*>, 1> m_VectorTypes = {
			{
				{"Vector2", "Vector3", "Vector4"}
			}
		};
	};

	class ShaderVectorDecomposeNode : public ShaderNode {
	public:
		ShaderVectorDecomposeNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		const std::vector<const char*>& GetVariantOptions(int index) const override { return m_VectorTypes[index]; };
		const std::vector<int>& GetVariantOptionsIndex() const override { return m_VectorTypesIndex; };
		void SetVariantOptionsIndex(int vectorIndex, int stringIndex) override { m_VectorTypesIndex[vectorIndex] = stringIndex; };

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Vector Decompose";
		inline static const char* s_OptionPath = "Vectors/Composition";
	private:
		std::vector<int> m_VectorTypesIndex = { 0 };

		static inline std::array<std::vector<const char*>, 1> m_VectorTypes = {
			{
				{"Vector2", "Vector3", "Vector4"}
			}
		};
	};;

	class ShaderDotProductNode : public ShaderNode {
	public:
		ShaderDotProductNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Dot Product";
		inline static const char* s_OptionPath = "Vectors/Operations";
	};

	class ShaderVectorOpsNode : public ShaderNode {
	public:
		ShaderVectorOpsNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		virtual const std::vector<const char*>& GetVariantOptions(int index) const { return m_Operations[index]; };
		virtual const std::vector<int>& GetVariantOptionsIndex() const { return m_OperationsIndex; };
		virtual void SetVariantOptionsIndex(int vectorIndex, int stringIndex) { m_OperationsIndex[vectorIndex] = stringIndex; };

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Vector Operations";
		inline static const char* s_OptionPath = "Vectors/Operations";

	private:
		std::vector<int> m_OperationsIndex = { 0, 0 };

		static inline std::array<std::vector<const char*>, 2> m_Operations = {
			{
				{"Vector2", "Vector3", "Vector4"},
				{"Add", "Subtract", "Multiply", "Divide"}
			}
		};
	};

	class ShaderConstantVec4Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec4Node();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }
		
		Variant& GetConstant() { return m_Constant; }
		void SetConstant(Variant constant) { m_Constant = std::move(constant); }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Vector4";
		inline static const char* s_OptionPath = "Vectors/Constants";

	private:
		Variant m_Constant = glm::vec4(1.0f);
	};

	class ShaderConstantVec3Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec3Node();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		Variant& GetConstant() override { return m_Constant; }
		void SetConstant(Variant constant) override { m_Constant = std::move(constant); }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Vector3";
		inline static const char* s_OptionPath = "Vectors/Constants";

	private:
		Variant m_Constant = glm::vec3(1.0f);
	};

	class ShaderConstantVec2Node : public ShaderNodeConstant {
	public:
		ShaderConstantVec2Node();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		Variant& GetConstant() { return m_Constant; }
		void SetConstant(Variant constant) { m_Constant = std::move(constant); }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Vector2";
		inline static const char* s_OptionPath = "Vectors/Constants";

	private:
		Variant m_Constant = glm::vec2(1.0f);
	};

	////////////////////////////////
	///////////Generic//////////////
	////////////////////////////////

	class ShaderConstantFloatNode : public ShaderNodeConstant {
	public:
		ShaderConstantFloatNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		Variant& GetConstant() { return m_Constant; }
		void SetConstant(Variant constant) { m_Constant = std::move(constant); }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Float";
		inline static const char* s_OptionPath = "Generic/Constants";

	private:
		Variant m_Constant = 0.0f;
	};

	class ShaderConstantIntNode : public ShaderNodeConstant {
	public:
		ShaderConstantIntNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		Variant& GetConstant() { return m_Constant; }
		void SetConstant(Variant constant) { m_Constant = std::move(constant); }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Int";
		inline static const char* s_OptionPath = "Generic/Constants";

	private:
		Variant m_Constant = 0;
	};

	class ShaderConstantBoolNode : public ShaderNodeConstant {
	public:
		ShaderConstantBoolNode();

		const char* GetName() const override { return s_Name; }
		const char* GetOptionPath() const override { return s_OptionPath; }

		Variant& GetConstant() { return m_Constant; }
		void SetConstant(Variant constant) { m_Constant = std::move(constant); }

		std::string GenerateCode(std::string* outputVars, std::string* inputVars) const override;
	public:
		inline static const char* s_Name = "Constanct Bool";
		inline static const char* s_OptionPath = "Generic/Constants";

	private:
		Variant m_Constant = false;
	};
}
