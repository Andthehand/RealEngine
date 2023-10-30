#pragma once
#include <vector>
#include <string>
#include <unordered_set>

#include <imgui_node_editor.h>

#include "RealEngine/Renderer/Texture.h"
#include "Nodes/ShaderNode.h"

namespace RealEngine {
	struct Link {
		ImNode::LinkId Id;
		Pin*  InputPin;
		Pin*  OutputPin;
	};

	class ShaderCreatePanel {
	public:
		ShaderCreatePanel();

		ImColor GetIconColor(PinType type);
		void DrawPinIcon(const Pin& pin, bool connected, int alpha);

		void OnImGuiRender();
		void HandleInteraction();

		void RecursiveSearch(const ShaderNode* currentNode, std::string& shaderCode, std::unordered_set<uint64_t>* nodeTracking);
		void Compile();

		Pin* FindPin(ImNode::PinId id);
		Link* FindPinLink(ImNode::PinId id);
		bool IsPinLinked(ImNode::PinId id);

		//This has to be called every time a node is added after first init because the node pointers inside of the pins will be corrupted
		void BuildNodes();

	private:
		Ref<Texture2D> m_HeaderBackground;

		ImNode::EditorContext* m_Context = nullptr;

		std::vector<Ref<ShaderNode>> m_Nodes;
		ImVector<Link>   m_Links;

		int              m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
	};
}
