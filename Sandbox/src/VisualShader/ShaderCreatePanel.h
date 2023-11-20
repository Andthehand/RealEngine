#pragma once
#include <vector>
#include <string>
#include <unordered_set>

//For some reason I have to include this or else there will be a linking error
#include <spdlog/fmt/fmt.h>

#include <imgui_node_editor.h>

#include "RealEngine/Utils/StringBuilder.h"

#include "RealEngine/Renderer/Texture.h"
#include "RealEngine/Renderer/Shader.h"
#include "Nodes/ShaderNode.h"

namespace RealEngine {
	struct Link {
		ImNode::LinkId Id;
		Pin* InputPin;
		Pin* OutputPin;
	};

	class ShaderCreatePanel {
	public:
		ShaderCreatePanel();

		ImColor GetIconColor(PinType type);
		void DrawPinIcon(const Pin& pin, bool connected, int alpha);

		void OnImGuiRender();
		void HandleInteraction();

		void OnUpdate();

		void RecursiveSearch(const ShaderNode* currentNode, StringBuilder& shaderCode, StringBuilder& globalCode, std::unordered_set<uint64_t>* nodeTracking);
		void Compile();

		Pin* FindPin(ImNode::PinId id);
		Link* FindPinLink(ImNode::PinId id);
		bool IsPinLinked(ImNode::PinId id);

		//This has to be called every time a node is added because the node pointers inside of the pins will be corrupted
		void BuildNodes();

		//Temp
		bool m_QueuedCompile = false;
		Ref<Shader> m_PreviewShader;
	private:
		template<class CustomNode>
		void RegisterNodeType() {
			m_NodeTypes.emplace_back(CustomNode::s_Name, []() { return CreateRef<CustomNode>(); });
		}

	private:
		Ref<Texture2D> m_HeaderBackground;

		ImNode::EditorContext* m_Context = nullptr;

		std::vector<Ref<ShaderNode>> m_Nodes;
		ImVector<Link> m_Links;

		std::string m_Tooltip;

		std::vector<std::pair<std::string, std::function<Ref<ShaderNode>()>>> m_NodeTypes;

		int m_NextLinkId = 100;
	};
}
