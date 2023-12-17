#include "ShaderNodes.h"

namespace RealEngine {
	VertexShaderOutputNode::VertexShaderOutputNode() {
		Inputs.emplace_back("UV", PinType::Vector2);
		Inputs.emplace_back("Vertex", PinType::Vector3);
	}

	//TODO: Implement this
	std::string VertexShaderOutputNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;
		
		if (!inputVars[0].empty())
			code += "\tv_UV = " + inputVars[0] + ";\n";
		else
			code += "\tv_UV = a_UV;\n";

		if(!inputVars[1].empty())
			code += "\tgl_Position = " + inputVars[1] + ";\n";
		else
			code += "\tgl_Position = vec4(a_Position, 1.0);\n";

		return code;
	}

	FragmentShaderOutputNode::FragmentShaderOutputNode()
		: ShaderNode() {
		Inputs.emplace_back("Color", PinType::Vector4);
		Inputs.emplace_back("Normal", PinType::Vector3);
	}

	std::string FragmentShaderOutputNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;
		//Color
		if (!inputVars[0].empty())
			code += "\to_Color = " + inputVars[0] + ";\n";

		//Normal
		std::string normal;
		if (!inputVars[1].empty()) 
			code += "\to_Normal = " + inputVars[1] + ";\n";

		return code;
	}

	ShaderTextureNode::ShaderTextureNode()
		: ShaderNode() {
		Inputs.emplace_back("Sampler2D", PinType::Sampler2D);
		Inputs.emplace_back("UV", PinType::Vector2);
		Inputs.emplace_back("LOD", PinType::Float);
		Outputs.emplace_back("Color", PinType::Vector4);
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

	std::string ShaderTextureNode::GenerateGlobalCode(std::string* inputVars, std::vector<std::string>* defines) const {
		std::string code;

		if (inputVars[0].empty()) {
			code += "uniform sampler2D " + GetUniformName(&Inputs[0]) + ";\n";
		}

		if(inputVars[1].empty())
			defines->emplace_back("IMPLEMENTUV");
		
		return code;
	}

	ShaderConstantVec4Node::ShaderConstantVec4Node() 
		: ShaderNodeConstant() {
		Outputs.emplace_back("Vector4", PinType::Vector4);
	}

	std::string ShaderConstantVec4Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantVec3Node::ShaderConstantVec3Node()
		: ShaderNodeConstant() {
		Outputs.emplace_back("Vector3", PinType::Vector3);
	}

	std::string ShaderConstantVec3Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantVec2Node::ShaderConstantVec2Node()
		: ShaderNodeConstant() {
		Outputs.emplace_back("Vector2", PinType::Vector2);
	}

	std::string ShaderConstantVec2Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantFloatNode::ShaderConstantFloatNode()
		: ShaderNodeConstant() {
		Outputs.emplace_back("Float", PinType::Float);
	}

	std::string ShaderConstantFloatNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantIntNode::ShaderConstantIntNode()
		: ShaderNodeConstant() {
		Outputs.emplace_back("Int", PinType::Int);
	}

	std::string ShaderConstantIntNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantBoolNode::ShaderConstantBoolNode()
		: ShaderNodeConstant() {
		Outputs.emplace_back("Bool", PinType::Bool);
	}

	std::string ShaderConstantBoolNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}
	
	ShaderDotProductNode::ShaderDotProductNode() {
		Inputs.emplace_back("A", PinType::Vector3);
		Inputs.emplace_back("B", PinType::Vector3);
		Outputs.emplace_back("Dot", PinType::Float);
	}

	std::string ShaderDotProductNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = dot(" + inputVars[0] + ", " + inputVars[1] + ");\n";
	}

	ShaderVectorOpsNode::ShaderVectorOpsNode() {
		//Type
		Content.emplace_back(ContentType::ComboBox);
		//Operation
		Content.emplace_back(ContentType::ComboBox);

		Inputs.emplace_back("A", PinType::Vector3);
		Inputs.emplace_back("B", PinType::Vector3);
		Outputs.emplace_back("Result", PinType::Vector3);
	}

	std::string ShaderVectorOpsNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if(m_Operations[1][m_OperationsIndex[1]] == "Add")
			code += "\t" + outputVars[0] + " = " + inputVars[2] + " + " + inputVars[3] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Subtract")
			code += "\t" + outputVars[0] + " = " + inputVars[2] + " - " + inputVars[3] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Multiply")
			code += "\t" + outputVars[0] + " = " + inputVars[2] + " * " + inputVars[3] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Divide")
			code += "\t" + outputVars[0] + " = " + inputVars[2] + " / " + inputVars[3] + ";\n";

		return code;
	}

	ShaderInputNode::ShaderInputNode() {
		Content.emplace_back(ContentType::ComboBox);

		Outputs.emplace_back("Result", PinType::Vector2);
	}

	std::string ShaderInputNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if (m_InputTypes[0][m_InputTypesIndex[0]] == "Position")
			code += "\t" + outputVars[0] + " = a_Position;\n";
		else if (m_InputTypes[0][m_InputTypesIndex[0]] == "UV")
			code += "\t" + outputVars[0] + " = v_UV;\n";

		return code;
	}
	std::string ShaderInputNode::GenerateGlobalCode(std::string* inputVars, std::vector<std::string>* defines) const {
		if (m_InputTypes[0][m_InputTypesIndex[0]] == "UV")
			defines->emplace_back("IMPLEMENTUV");

		return std::string();
	}

	ShaderVectorComposeNode::ShaderVectorComposeNode() {
		Content.emplace_back(ContentType::ComboBox);
		m_VectorTypesIndex[0] = 2;

		Inputs.emplace_back("X", PinType::Float);
		Inputs.emplace_back("Y", PinType::Float);
		Inputs.emplace_back("Z", PinType::Float);
		Inputs.emplace_back("W", PinType::Float);

		Outputs.emplace_back("Results", PinType::Vector4);
	}

	void ShaderVectorComposeNode::SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
		m_VectorTypesIndex[vectorIndex] = stringIndex;

		ChangePinTypes();
	}

	std::string ShaderVectorComposeNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector2")
			code += "\t" + outputVars[0] + " = vec2(" + inputVars[0] + ", " + inputVars[1] + ");\n";
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector3")
			code += "\t" + outputVars[0] + " = vec3(" + inputVars[0] + ", " + inputVars[1] + ", " + inputVars[3] + ");\n";
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector4")
			code += "\t" + outputVars[0] + " = vec4(" + inputVars[0] + ", " + inputVars[1] + ", " + inputVars[2] + ", " + inputVars[3] + ");\n";

		return code;
	}

	void ShaderVectorComposeNode::ChangePinTypes() {
		ScopeUniqueIdChange ID(ID.Get() + 1);

		Inputs.clear();
		if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector2") {
			Inputs.emplace_back("X", PinType::Float);
			Inputs.emplace_back("Y", PinType::Float);

			Outputs[0].Type = PinType::Vector2;
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector3") {
			Inputs.emplace_back("X", PinType::Float);
			Inputs.emplace_back("Y", PinType::Float);
			Inputs.emplace_back("Z", PinType::Float);

			Outputs[0].Type = PinType::Vector3;
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector4") {
			Inputs.emplace_back("X", PinType::Float);
			Inputs.emplace_back("Y", PinType::Float);
			Inputs.emplace_back("Z", PinType::Float);
			Inputs.emplace_back("W", PinType::Float);

			Outputs[0].Type = PinType::Vector4;
		}
	}

	ShaderVectorDecomposeNode::ShaderVectorDecomposeNode() {
		Content.emplace_back(ContentType::ComboBox);

		Inputs.emplace_back("Vector", PinType::Vector2);
		Outputs.emplace_back("X", PinType::Float);
		Outputs.emplace_back("Y", PinType::Float);
	}

	std::string ShaderVectorDecomposeNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector2") {
			code += "\t" + outputVars[0] + " = " + inputVars[0] + ".x;\n";
			code += "\t" + outputVars[1] + " = " + inputVars[0] + ".y;\n";
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector3") {
			code += "\t" + outputVars[0] + " = " + inputVars[0] + ".x;\n";
			code += "\t" + outputVars[1] + " = " + inputVars[0] + ".y;\n";
			code += "\t" + outputVars[2] + " = " + inputVars[0] + ".z;\n";
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector4") {
			code += "\t" + outputVars[0] + " = " + inputVars[0] + ".x;\n";
			code += "\t" + outputVars[1] + " = " + inputVars[0] + ".y;\n";
			code += "\t" + outputVars[2] + " = " + inputVars[0] + ".z;\n";
			code += "\t" + outputVars[3] + " = " + inputVars[0] + ".w;\n";
		}

		return code;
	}
}

