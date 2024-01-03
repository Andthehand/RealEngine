#include "ShaderNodes.h"

namespace RealEngine {
	VertexShaderOutputNode::VertexShaderOutputNode() {
		Inputs.emplace_back("UV", PinType::Vector2);
		Inputs.emplace_back("Vertex", PinType::Vector4);
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
		: ShaderNode() {
		Outputs.emplace_back("Vector4", PinType::Vector4, true);
	}

	std::string ShaderConstantVec4Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantVec3Node::ShaderConstantVec3Node()
		: ShaderNode() {
		Outputs.emplace_back("Vector3", PinType::Vector3, true);
	}

	std::string ShaderConstantVec3Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantVec2Node::ShaderConstantVec2Node()
		: ShaderNode() {
		Outputs.emplace_back("Vector2", PinType::Vector2, true);
	}

	std::string ShaderConstantVec2Node::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantFloatNode::ShaderConstantFloatNode()
		: ShaderNode() {
		Outputs.emplace_back("Float", PinType::Float, true);
	}

	std::string ShaderConstantFloatNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantIntNode::ShaderConstantIntNode()
		: ShaderNode() {
		Outputs.emplace_back("Int", PinType::Int, true);
	}

	std::string ShaderConstantIntNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
	}

	ShaderConstantBoolNode::ShaderConstantBoolNode()
		: ShaderNode() {
		Outputs.emplace_back("Bool", PinType::Bool, true);
	}

	std::string ShaderConstantBoolNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		return "\t" + outputVars[0] + " = " + (std::string)m_Constant + "; \n";
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

	void ShaderVectorOpsNode::SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
		if (m_OperationsIndex[vectorIndex] != stringIndex) {
			m_OperationsIndex[vectorIndex] = stringIndex;
			if(vectorIndex == 0)
				ChangePinTypes();
		}
	}

	std::string ShaderVectorOpsNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if(m_Operations[1][m_OperationsIndex[1]] == "Add")
			code += "\t" + outputVars[0] + " = " + inputVars[0] + " + " + inputVars[1] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Subtract")
			code += "\t" + outputVars[0] + " = " + inputVars[0] + " - " + inputVars[1] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Multiply")
			code += "\t" + outputVars[0] + " = " + inputVars[0] + " * " + inputVars[1] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Divide")
			code += "\t" + outputVars[0] + " = " + inputVars[0] + " / " + inputVars[1] + ";\n";

		return code;
	}

	void ShaderVectorOpsNode::ChangePinTypes() {
		if (m_Operations[0][m_OperationsIndex[0]] == "Vector2") {
			Inputs[0].Type = PinType::Vector2;
			Inputs[1].Type = PinType::Vector2;

			Outputs[0].Type = PinType::Vector2;
		}
		else if (m_Operations[0][m_OperationsIndex[0]] == "Vector3") {
			Inputs[0].Type = PinType::Vector3;
			Inputs[1].Type = PinType::Vector3;

			Outputs[0].Type = PinType::Vector3;
		}
		else if (m_Operations[0][m_OperationsIndex[0]] == "Vector4") {
			Inputs[0].Type = PinType::Vector4;
			Inputs[1].Type = PinType::Vector4;

			Outputs[0].Type = PinType::Vector4;
		}
	}

	ShaderInputNode::ShaderInputNode() {
		Content.emplace_back(ContentType::ComboBox);

		Outputs.emplace_back("Result", PinType::Vector3);
	}

	void ShaderInputNode::SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
		if (m_InputTypesIndex[vectorIndex] != stringIndex) {
			m_InputTypesIndex[vectorIndex] = stringIndex;
			if (vectorIndex == 0)
				ChangePinTypes();
		}
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

	void ShaderInputNode::ChangePinTypes() {
		if (m_InputTypes[0][m_InputTypesIndex[0]] == "Position") {
			Outputs[0].Type = PinType::Vector3;
		}
		else if (m_InputTypes[0][m_InputTypesIndex[0]] == "UV") {
			Outputs[0].Type = PinType::Vector2;
		}
	}

	ShaderVectorComposeNode::ShaderVectorComposeNode() {
		Content.emplace_back(ContentType::ComboBox);
		m_VectorTypesIndex[0] = 2;

		Inputs.emplace_back("X", PinType::Float, true);
		Inputs.emplace_back("Y", PinType::Float, true);
		Inputs.emplace_back("Z", PinType::Float, true);
		Inputs.emplace_back("W", PinType::Float, true);

		Outputs.emplace_back("Results", PinType::Vector4);
	}

	void ShaderVectorComposeNode::SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
		if (m_VectorTypesIndex[vectorIndex] != stringIndex) {
			m_VectorTypesIndex[vectorIndex] = stringIndex;
			ChangePinTypes();
		}
	}

	std::string ShaderVectorComposeNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		std::vector<std::string> vars;
		vars.resize(Inputs.size());
		for (int i = 0; i < vars.size(); i++) {
			if (inputVars[i].empty())
				vars[i] = m_Constant[i];
			else
				vars[i] = inputVars[i];
		}

		if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector2") {
			code += "\t" + outputVars[0] + " = vec2(" + vars[0] + ", " + vars[1] + ");\n";
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector3") {
			code += "\t" + outputVars[0] + " = vec3(" + vars[0] + ", " + vars[1] + ", " + vars[2] + ");\n";
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector4") {
			code += "\t" + outputVars[0] + " = vec4(" + vars[0] + ", " + vars[1] + ", " + vars[2] + ", " + vars[3] + ");\n";
		}

		return code;
	}

	void ShaderVectorComposeNode::ChangePinTypes() {
		ScopeUniqueIdChange ID(ID.Get() + 1);

		Inputs.clear();
		if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector2") {
			Inputs.emplace_back("X", PinType::Float, true);
			Inputs.emplace_back("Y", PinType::Float, true);

			Outputs[0].Type = PinType::Vector2;
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector3") {
			Inputs.emplace_back("X", PinType::Float, true);
			Inputs.emplace_back("Y", PinType::Float, true);
			Inputs.emplace_back("Z", PinType::Float, true);

			Outputs[0].Type = PinType::Vector3;
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector4") {
			Inputs.emplace_back("X", PinType::Float, true);
			Inputs.emplace_back("Y", PinType::Float, true);
			Inputs.emplace_back("Z", PinType::Float, true);
			Inputs.emplace_back("W", PinType::Float, true);

			Outputs[0].Type = PinType::Vector4;
		}

		BuildNode();
	}

	ShaderVectorDecomposeNode::ShaderVectorDecomposeNode() {
		Content.emplace_back(ContentType::ComboBox);
		m_VectorTypesIndex[0] = 2;

		Outputs.emplace_back("X", PinType::Float);
		Outputs.emplace_back("Y", PinType::Float);
		Outputs.emplace_back("Z", PinType::Float);
		Outputs.emplace_back("W", PinType::Float);

		Inputs.emplace_back("Vector", PinType::Vector4);
	}

	void ShaderVectorDecomposeNode::SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
		if (m_VectorTypesIndex[vectorIndex] != stringIndex) {
			m_VectorTypesIndex[vectorIndex] = stringIndex;
			ChangePinTypes();
		}
	}

	std::string ShaderVectorDecomposeNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector2") {
			if(!outputVars[0].empty())
				code += "\t" + outputVars[0] + " = " + inputVars[0] + ".x;\n";
			if(!outputVars[1].empty())
				code += "\t" + outputVars[1] + " = " + inputVars[0] + ".y;\n";
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector3") {
			if(!outputVars[0].empty())
				code += "\t" + outputVars[0] + " = " + inputVars[0] + ".x;\n";
			if(!outputVars[1].empty())
				code += "\t" + outputVars[1] + " = " + inputVars[0] + ".y;\n";
			if(!outputVars[2].empty())
				code += "\t" + outputVars[2] + " = " + inputVars[0] + ".z;\n";
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector4") {
			if(!outputVars[0].empty())
				code += "\t" + outputVars[0] + " = " + inputVars[0] + ".x;\n";
			if(!outputVars[1].empty())
				code += "\t" + outputVars[1] + " = " + inputVars[0] + ".y;\n";
			if(!outputVars[2].empty())
				code += "\t" + outputVars[2] + " = " + inputVars[0] + ".z;\n";
			if(!outputVars[3].empty())
				code += "\t" + outputVars[3] + " = " + inputVars[0] + ".w;\n";
		}

		return code;
	}

	void ShaderVectorDecomposeNode::ChangePinTypes() {
		ScopeUniqueIdChange ID(ID.Get() + 1);

		Outputs.clear();
		if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector2") {
			Outputs.emplace_back("X", PinType::Float);
			Outputs.emplace_back("Y", PinType::Float);

			Inputs[0].Type = PinType::Vector2;
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector3") {
			Outputs.emplace_back("X", PinType::Float);
			Outputs.emplace_back("Y", PinType::Float);
			Outputs.emplace_back("Z", PinType::Float);

			Inputs[0].Type = PinType::Vector3;
		}
		else if (m_VectorTypes[0][m_VectorTypesIndex[0]] == "Vector4") {
			Outputs.emplace_back("X", PinType::Float);
			Outputs.emplace_back("Y", PinType::Float);
			Outputs.emplace_back("Z", PinType::Float);
			Outputs.emplace_back("W", PinType::Float);

			Inputs[0].Type = PinType::Vector4;
		}

		BuildNode();
	}

	ShaderGenericOpsNode::ShaderGenericOpsNode() {
		//Type
		Content.emplace_back(ContentType::ComboBox);
		//Operation
		Content.emplace_back(ContentType::ComboBox);

		Inputs.emplace_back("A", PinType::Float);
		Inputs.emplace_back("B", PinType::Float);
		Outputs.emplace_back("Result", PinType::Float);
	}
	
	void ShaderGenericOpsNode::SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
		if (m_OperationsIndex[vectorIndex] != stringIndex) {
			m_OperationsIndex[vectorIndex] = stringIndex;
			if (vectorIndex == 0)
				ChangePinTypes();
		}
	}

	std::string ShaderGenericOpsNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if (m_Operations[1][m_OperationsIndex[1]] == "Add")
			code += "\t" + outputVars[0] + " = " + inputVars[0] + " + " + inputVars[1] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Subtract")
			code += "\t" + outputVars[0] + " = " + inputVars[0] + " - " + inputVars[1] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Multiply")
			code += "\t" + outputVars[0] + " = " + inputVars[0] + " * " + inputVars[1] + ";\n";
		else if (m_Operations[1][m_OperationsIndex[1]] == "Divide")
			code += "\t" + outputVars[0] + " = " + inputVars[0] + " / " + inputVars[1] + ";\n";

		return code;
	}

	void ShaderGenericOpsNode::ChangePinTypes() {
		if (m_Operations[0][m_OperationsIndex[0]] == "Float") {
			Inputs[0].Type = PinType::Float;
			Inputs[1].Type = PinType::Float;

			Outputs[0].Type = PinType::Float;
		}
		else if (m_Operations[0][m_OperationsIndex[0]] == "Int") {
			Inputs[0].Type = PinType::Int;
			Inputs[1].Type = PinType::Int;

			Outputs[0].Type = PinType::Int;
		}
		else if (m_Operations[0][m_OperationsIndex[0]] == "Bool") {
			Inputs[0].Type = PinType::Bool;
			Inputs[1].Type = PinType::Bool;

			Outputs[0].Type = PinType::Bool;
		}
	}

	ShaderVectorFuncNode::ShaderVectorFuncNode() {
		//Type
		Content.emplace_back(ContentType::ComboBox);
		//Operation
		Content.emplace_back(ContentType::ComboBox);

		Inputs.emplace_back("A", PinType::Vector3);
		Outputs.emplace_back("Result", PinType::Vector3);
	}

	void ShaderVectorFuncNode::SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
		if (m_FunctionsIndex[vectorIndex] != stringIndex) {
			m_FunctionsIndex[vectorIndex] = stringIndex;
			if (vectorIndex == 0)
				ChangePinTypes();
		}
	}

	std::string ShaderVectorFuncNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if(m_Functions[1][m_FunctionsIndex[1]] == "Normalize")
			code += "\t" + outputVars[0] + " = normalize(" + inputVars[0] + ");\n";
		else if (m_Functions[1][m_FunctionsIndex[1]] == "Length")
			code += "\t" + outputVars[0] + " = length(" + inputVars[0] + ");\n";
		else if (m_Functions[1][m_FunctionsIndex[1]] == "Sine")
			code += "\t" + outputVars[0] + " = sin(" + inputVars[0] + ");\n";
		else if (m_Functions[1][m_FunctionsIndex[1]] == "Cosine")
			code += "\t" + outputVars[0] + " = cos(" + inputVars[0] + ");\n";
		else if (m_Functions[1][m_FunctionsIndex[1]] == "Tangent")
			code += "\t" + outputVars[0] + " = tan(" + inputVars[0] + ");\n";

		return code;
	}

	void ShaderVectorFuncNode::ChangePinTypes() {
		if (m_Functions[0][m_FunctionsIndex[0]] == "Vector2") {
			Inputs[0].Type = PinType::Vector2;
			Inputs[1].Type = PinType::Vector2;

			Outputs[0].Type = PinType::Vector2;
		}
		else if (m_Functions[0][m_FunctionsIndex[0]] == "Vector3") {
			Inputs[0].Type = PinType::Vector3;
			Inputs[1].Type = PinType::Vector3;

			Outputs[0].Type = PinType::Vector3;
		}
		else if (m_Functions[0][m_FunctionsIndex[0]] == "Vector4") {
			Inputs[0].Type = PinType::Vector4;
			Inputs[1].Type = PinType::Vector4;

			Outputs[0].Type = PinType::Vector4;
		}
	}

	ShaderGenericFuncNode::ShaderGenericFuncNode() {\
		//Type
		Content.emplace_back(ContentType::ComboBox);
		//Operation
		Content.emplace_back(ContentType::ComboBox);

		Inputs.emplace_back("A", PinType::Float);
		Outputs.emplace_back("Result", PinType::Float);
	}

	void ShaderGenericFuncNode::SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
		if (m_FunctionsIndex[vectorIndex] != stringIndex) {
			m_FunctionsIndex[vectorIndex] = stringIndex;
			if (vectorIndex == 0)
				ChangePinTypes();
		}
	}
	std::string ShaderGenericFuncNode::GenerateCode(std::string* outputVars, std::string* inputVars) const {
		std::string code;

		if (m_Functions[1][m_FunctionsIndex[1]] == "Normalize")
			code += "\t" + outputVars[0] + " = normalize(" + inputVars[0] + ");\n";
		else if (m_Functions[1][m_FunctionsIndex[1]] == "Length")
			code += "\t" + outputVars[0] + " = length(" + inputVars[0] + ");\n";
		else if (m_Functions[1][m_FunctionsIndex[1]] == "Sine")
			code += "\t" + outputVars[0] + " = sin(" + inputVars[0] + ");\n";
		else if (m_Functions[1][m_FunctionsIndex[1]] == "Cosine")
			code += "\t" + outputVars[0] + " = cos(" + inputVars[0] + ");\n";
		else if (m_Functions[1][m_FunctionsIndex[1]] == "Tangent")
			code += "\t" + outputVars[0] + " = tan(" + inputVars[0] + ");\n";

		return code;
	}

	void ShaderGenericFuncNode::ChangePinTypes() {
		if (m_Functions[0][m_FunctionsIndex[0]] == "Float") {
			Inputs[0].Type = PinType::Float;
			Inputs[1].Type = PinType::Float;

			Outputs[0].Type = PinType::Float;
		}
		else if (m_Functions[0][m_FunctionsIndex[0]] == "Int") {
			Inputs[0].Type = PinType::Int;
			Inputs[1].Type = PinType::Int;

			Outputs[0].Type = PinType::Int;
		}
		else if (m_Functions[0][m_FunctionsIndex[0]] == "Bool") {
			Inputs[0].Type = PinType::Bool;
			Inputs[1].Type = PinType::Bool;

			Outputs[0].Type = PinType::Bool;
		}
	}
}

