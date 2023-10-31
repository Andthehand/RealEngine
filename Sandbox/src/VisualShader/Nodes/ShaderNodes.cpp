#include "ShaderNodes.h"

namespace RealEngine {
	FragmentShaderOutputNode::FragmentShaderOutputNode()
		: ShaderNode("Fragment Output") {
		Inputs.emplace_back(GetNextId(), "Color", PinType::Vector4);
		Inputs.emplace_back(GetNextId(), "Normal", PinType::Vector3);
	}

	//TODO: Implement this
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

	ShaderTextureNode::ShaderTextureNode()
		: ShaderNode("Texture2D") {
		Inputs.emplace_back(GetNextId(), "UV", PinType::Vector2);
		Inputs.emplace_back(GetNextId(), "LOD", PinType::Float);
		Inputs.emplace_back(GetNextId(), "Sampler2D", PinType::Sampler2D);
		Outputs.emplace_back(GetNextId(), "Color", PinType::Vector4);
	}

	std::string ShaderTextureNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		//Sampler2D ID
		std::string id;
		if (inputVars[0].empty()) {
			//Change this to generate its own sampler2D id
			id = "0";
		}
		else {
			id = inputVars[0];
		}

		//UV
		std::string uv;
		if (inputVars[2].empty()) {
			uv = "UV";
		}
		else {
			uv = inputVars[1];
		}
		
		//LOD
		if (inputVars[1].empty()) {
			return "\t" + outputVars[0] + " = texture(" + id + ", " + uv + ");\n";
		}
		else {
			return "\t" + outputVars[0] + " = texture(" + id + ", " + uv + ", " + inputVars[1] + ");\n";
		}
	}

	ShaderConstantVec4Node::ShaderConstantVec4Node() 
		: ShaderNode("Constanct Vector 4") {
		Outputs.emplace_back(GetNextId(), "Vector4", PinType::Vector4);
	}

	std::string ShaderConstantVec4Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = vec4(255.0, 255.0, 255.0, 255.0);\n";
	}
}

