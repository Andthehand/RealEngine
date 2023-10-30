#include "ShaderNodes.h"

namespace RealEngine {
	FragmentShaderOutputNode::FragmentShaderOutputNode()
		: ShaderNode("Fragment Output") {
		Inputs.emplace_back(GetNextId(), "Color", PinType::Float);
		Inputs.emplace_back(GetNextId(), "Normal", PinType::Float);
	}

	//TODO: Implement this
	std::string FragmentShaderOutputNode::GenerateCode(std::string* outputVars, std::string* inputVars) {
		return std::string();
	}

	ShaderTextureNode::ShaderTextureNode()
		: ShaderNode("Texture2D") {
		Inputs.emplace_back(GetNextId(), "UV", PinType::Float);
		Inputs.emplace_back(GetNextId(), "LOD", PinType::Float);
		Inputs.emplace_back(GetNextId(), "Sampler2D", PinType::Float);
		Outputs.emplace_back(GetNextId(), "Color", PinType::Float);
	}

	std::string ShaderTextureNode::GenerateCode(std::string* outputVars, std::string* inputVars) {
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
			id = "UV";
		}
		else {
			id = inputVars[1];
		}
		
		//LOD
		if (inputVars[1].empty()) {
			return "\t" + outputVars[0] + " = texture(" + id + ", " + uv + ");\n";
		}
		else {
			return "\t" + outputVars[0] + " = texture(" + id + ", " + uv + ", " + inputVars[1] + ");\n";
		}
	}
}

