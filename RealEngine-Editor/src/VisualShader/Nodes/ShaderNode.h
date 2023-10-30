#pragma once
#include <string>
#include <vector>

#include "../ImNode.h"

namespace RealEngine {
	class ShaderNode;

	enum class PinType {
		Bool,
		Int,
		Float,
		Vector2,
		Vector3,
		String
	};

	struct Pin {
		Pin(ImNode::PinId id, const char* name, PinType type)
			: ID(id), Name(name), Type(type), Kind(PinKind::Input) {}

		ImNode::PinId ID;
		ShaderNode* Node = nullptr;
		PinType Type;
		PinKind Kind;

		std::string Name;
	};

	class ShaderNode {
	public:
		ShaderNode(const char* name)
			: ID(GetNextId()), Name(name) {}

		void BuildNode() {
			for (Pin& input : Inputs) {
				input.Kind = PinKind::Input;
				input.Node = this;
			}

			for (Pin& output : Outputs) {
				output.Kind = PinKind::Output;
				output.Node = this;
			}
		}

		virtual std::string GenerateCode(std::string* outputVars, std::string* inputVars) const = 0;

		ImNode::NodeId ID;

		std::string Name;

		std::vector<Pin*> ConnectedInputs;
		std::vector<Pin> Inputs;

		std::vector<Pin*> ConnectedOutputs;
		std::vector<Pin> Outputs;
	};
}
