#pragma once
#include <vector>
#include <string>

#include <imgui_node_editor.h>

#include "RealEngine/Renderer/Texture.h"

namespace ImNode = ax::NodeEditor;
using PinKind = ax::NodeEditor::PinKind;

namespace RealEngine {
	//Forward declaration
	struct Node;

	enum class PinType {
		Bool,
		Int,
		Float,
		Vector2,
		Vector3,
		String,

		//This is to tell what function connects to what function
		Flow
	};

	struct Pin {
		Pin(ImNode::PinId id, const char* name, PinType type)
			: ID(id), Name(name), Type(type), Kind(PinKind::Input) {}

		ImNode::PinId ID;
		Node* Node;
		PinType Type;
		PinKind Kind;

		std::string Name;
	};

	struct Node {
		Node(ImNode::NodeId id, const char* name)
			: ID(id), Name(name) {}

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

		ImNode::NodeId ID;

		std::string Name;

		std::vector<Pin> Inputs;
		std::vector<Pin> Outputs;
	};

	struct Link {
		ImNode::LinkId Id;
		ImNode::PinId  InputId;
		ImNode::PinId  OutputId;
	};

	class ShaderCreatePanel {
	public:
		ShaderCreatePanel();

		ImColor GetIconColor(PinType type);
		void DrawPinIcon(const Pin& pin, bool connected, int alpha);

		void OnImGuiRender();
		void HandleInteraction();

		void Compile();

		Pin* FindPin(ImNode::PinId id);
		Link* FindPinLink(ImNode::PinId id);
		bool IsPinLinked(ImNode::PinId id);

		//This has to be called ever time a node is added after first init because the node pointers inside of the pins will be corrupted
		void BuildNodes();

		inline int GetNextId() { return m_UniqueId++; }
	private:
		Ref<Texture2D> m_HeaderBackground;

		ImNode::EditorContext* m_Context = nullptr;

		int m_UniqueId = 1;
		std::vector<Node>   m_Nodes;
		ImVector<Link>   m_Links;

		ImVector<ImNode::PinId>   m_FlowStartIDs;
		
		int              m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
	};
}
