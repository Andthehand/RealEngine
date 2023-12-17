#pragma once
#include <string>
#include <vector>

#include <RealEngine/Utils/Variant.h>

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

	enum class ContentType {
		ComboBox
	};

	struct Pin {
		Pin(const char* name, PinType type)
			: ID(UniqueId::GetNextId()), Name(name), Type(type), Kind(PinKind::Input) {}

		~Pin() {
			Dissconnect();
		}

		bool IsConnected() const { return ConnectedPin; }

		void Connect(Pin* pin) {
			ConnectedPin = pin;
			pin->ConnectedPin = this;
		}

		void Dissconnect() {
			if (IsConnected()) {
				ConnectedPin->ConnectedPin = nullptr;
				ConnectedPin = nullptr;
			}
		}

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
			: ID(UniqueId::GetNextId()) {}

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

		virtual const std::vector<const char*>& GetVariantOptions(int index) const {
			RE_CORE_ASSERT(false); 
			return std::vector<const char*>();
		};
		virtual const std::vector<int>& GetVariantOptionsIndex() const { 
			RE_CORE_ASSERT(false);
			return std::vector<int>();
		};

		//Returns true if the node needs to be rebuilt
		virtual void SetVariantOptionsIndex(int vectorIndex, int stringIndex) {
			RE_CORE_ASSERT(false);
		};

		virtual std::string GenerateCode(std::string* outputVars, std::string* inputVars) const = 0;
		virtual std::string GenerateGlobalCode(std::string* inputVars, std::vector<std::string>* defines) const { return ""; }

		std::string GetUniformName(const Pin* pin) const {
			return "u_" + std::to_string(ID.Get()) + "_" + std::string(GetName()) + "_" + pin->Name;
		}
	public:
		ImNode::NodeId ID;

		std::vector<Pin> Inputs;
		std::vector<Pin> Outputs;
		std::vector<ContentType> Content;
	};

	class ShaderNodeConstant : public ShaderNode {
	public:
		ShaderNodeConstant()
			: ShaderNode() {}

		virtual Variant& GetConstant() = 0;
		virtual void SetConstant(Variant constant) = 0;
	};
}
