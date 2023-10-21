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
		String
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

		ImColor GetIconColor(PinType type);
		void DrawPinIcon(const Pin& pin, bool connected, int alpha);

		void OnImGuiRender();
		void HandleInteraction();

		Pin* FindPin(ImNode::PinId id);
		bool IsPinLinked(ImNode::PinId id);

		inline int GetNextId() { return m_UniqueId++; }
	private:
		Ref<Texture2D> m_HeaderBackground;

		ImNode::EditorContext* m_Context = nullptr;

		int m_UniqueId = 1;
		std::vector<Node>   m_Nodes;
		ImVector<Link>   m_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
		int                  m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
	};
}
