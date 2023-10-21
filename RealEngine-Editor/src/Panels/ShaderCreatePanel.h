#pragma once
#include <imgui_node_editor.h>

#include <vector>
#include <string>

namespace ImNode = ax::NodeEditor;
using PinKind = ax::NodeEditor::PinKind;

namespace RealEngine {
	//Forward declaration
	struct Node;

	struct Pin {
		Pin(ImNode::PinId id, const char* name)
			: ID(id), Name(name), Kind() {}

		ImNode::PinId ID;
		Node* Node;
		//This is just a default value and will be set later in the script
		PinKind Kind = PinKind::Input;

		std::string Name;
	};

	struct Node {
		Node(ImNode::NodeId id, const char* name)
			: ID(id), Name(name) {}

		void BuildNode() {
			for (auto& input : Inputs) {
				input.Kind = PinKind::Input;
				input.Node = this;
			}

			for (auto& output : Outputs) {
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

		void OnImGuiRender();
		void HandleInteraction();

		Pin* FindPin(ImNode::PinId id);

		inline int GetNextId() { return m_UniqueId++; }
	private:
		ImNode::EditorContext* m_Context = nullptr;

		int m_UniqueId = 1;
		std::vector<Node>   m_Nodes;
		ImVector<Link>   m_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
		int                  m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
	};
}
