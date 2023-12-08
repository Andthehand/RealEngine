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
		Vector4,

		//GLSL Specific Types
		Sampler2D,
	};

	struct Pin {
		Pin(ImNode::PinId id, const char* name, PinType type)
			: ID(id), Name(name), Type(type), Kind(PinKind::Input) {}

		ImNode::PinId ID;
		ShaderNode* Node = nullptr;
		PinType Type;
		PinKind Kind;

		//This isn't a vector because outputs only need to know if they are connected
		//and not with who they are connected with
		Pin* ConnectedPin = nullptr;

		std::string Name;
	};

	class ShaderNode {
	public:
		ShaderNode()
			: ID(GetNextId()) {}

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

		virtual const char* GetName() const = 0;
		virtual const char* GetOptionPath() const = 0;

		virtual std::string GenerateCode(std::string* outputVars, std::string* inputVars) const = 0;
		virtual std::string GenerateGlobalCode(std::string* inputVars) const { return ""; }

		std::string GetUniformName(const Pin* pin) const {
			return "u_" + std::to_string(ID.Get()) + "_" + std::string(GetName()) + "_" + pin->Name;
		}
	public:
		ImNode::NodeId ID;

		std::vector<Pin> Inputs;
		std::vector<Pin> Outputs;
	};

	class ShaderNodeConstant : public ShaderNode {
	public:
		ShaderNodeConstant()
			: ShaderNode() {}

		virtual void* GetConstant() const = 0;
		virtual void SetConstant(void* constant) = 0;
	};
}
