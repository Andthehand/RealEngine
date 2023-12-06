#include "ShaderNodes.h"

namespace RealEngine {
	VertexShaderOutputNode::VertexShaderOutputNode() {
		Inputs.emplace_back(GetNextId(), "Vertex", PinType::Vector3);
	}

	//TODO: Implement this
	std::string VertexShaderOutputNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "";
	}

	//TODO: Implement this
	std::string VertexShaderOutputNode::GenerateGlobalCode(std::string* inputVars) const {
		return "";
	}

	FragmentShaderOutputNode::FragmentShaderOutputNode()
		: ShaderNode() {
		Inputs.emplace_back(GetNextId(), "Color", PinType::Vector4);
		Inputs.emplace_back(GetNextId(), "Normal", PinType::Vector3);
	}

	std::string FragmentShaderOutputNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		//Color
		std::string color;
		if (!inputVars[0].empty())
			color = "\to_Color = " + inputVars[0] + ";\n";

		//Normal
		std::string normal;
		if (!inputVars[1].empty()) 
			normal = "\to_Normal = " + inputVars[1] + ";\n";

		return color + normal;
	}

	std::string FragmentShaderOutputNode::GenerateGlobalCode(std::string* inputVars) const {
		std::string code;
		
		if (!inputVars[0].empty())
			code += "out vec4 o_Color;\n";

		if (!inputVars[1].empty())
			code += "out vec3 o_Normal;\n";

		return code;
	}

	ShaderTextureNode::ShaderTextureNode()
		: ShaderNode() {
		Inputs.emplace_back(GetNextId(), "Sampler2D", PinType::Sampler2D);
		Inputs.emplace_back(GetNextId(), "UV", PinType::Vector2);
		Inputs.emplace_back(GetNextId(), "LOD", PinType::Float);
		Outputs.emplace_back(GetNextId(), "Color", PinType::Vector4);
	}

	std::string ShaderTextureNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		//Sampler2D ID
		std::string id;
		if (inputVars[0].empty()) {
			//Change this to generate its own sampler2D id
			id = GetUniformName(&Inputs[0]);
		}
		else {
			id = inputVars[0];
		}

		//UV
		std::string uv;
		if (inputVars[1].empty()) {
			uv = "v_UV";
		}
		else {
			uv = inputVars[1];
		}
		
		//LOD
		if (inputVars[2].empty()) {
			return "\t" + outputVars[0] + " = texture(" + id + ", " + uv + ");\n";
		}
		else {
			return "\t" + outputVars[0] + " = texture(" + id + ", " + uv + ", " + inputVars[1] + ");\n";
		}
	}

	std::string ShaderTextureNode::GenerateGlobalCode(std::string* inputVars) const {
		std::string code;

		if (inputVars[0].empty()) {
			code += "uniform sampler2D " + GetUniformName(&Inputs[0]) + ";\n";
		}

		if (inputVars[1].empty()) {
			code += "in vec2 v_UV;\n";
		}
		
		return code;
	}

	ShaderConstantVec4Node::ShaderConstantVec4Node() 
		: ShaderNodeConstant() {
		Outputs.emplace_back(GetNextId(), "Vector4", PinType::Vector4);
	}

	std::string ShaderConstantVec4Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + glm::to_string(m_Constant) + "; \n";
	}

	ShaderConstantVec3Node::ShaderConstantVec3Node()
		: ShaderNodeConstant() {
		Outputs.emplace_back(GetNextId(), "Vector3", PinType::Vector3);
	}

	std::string ShaderConstantVec3Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + glm::to_string(m_Constant) + "; \n";
	}

	ShaderConstantVec2Node::ShaderConstantVec2Node()
		: ShaderNodeConstant() {
		Outputs.emplace_back(GetNextId(), "Vector2", PinType::Vector2);
	}

	std::string ShaderConstantVec2Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + glm::to_string(m_Constant) + "; \n";
	}

	ShaderConstantFloatNode::ShaderConstantFloatNode()
		: ShaderNodeConstant() {
		Outputs.emplace_back(GetNextId(), "Float", PinType::Float);
	}

	std::string ShaderConstantFloatNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + std::to_string(m_Constant) + "; \n";
	}
}

