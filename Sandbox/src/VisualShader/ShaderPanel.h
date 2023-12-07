#pragma once
#include <vector>
#include <string>
#include <unordered_set>

//For some reason I have to include this or else there will be a linking error
#include <spdlog/fmt/fmt.h>

#include <imgui_node_editor.h>

#include "RealEngine/Utils/StringBuilder.h"
#include "RealEngine/Renderer/Texture.h"

#include "Nodes/ShaderNode.h"
#include "../Utils/NaryTree.h"

namespace RealEngine {
	struct Link {
		ImNode::LinkId Id;
		Pin* InputPin;
		Pin* OutputPin;
	};

	struct CreateOptions {
		CreateOptions(std::string name, std::function<Ref<ShaderNode>()> createFunction = nullptr) 
			: Name(name), CreateFunction(createFunction) {}

		//The Name also is the category if CreateFunction is empty
		std::string Name;

		std::function<Ref<ShaderNode>()> CreateFunction;
	};

	class ShaderPanel {
	public:
		ShaderPanel(const char* type);

		void SetHeaderBackground(const Ref<Texture2D>& texture) { m_HeaderBackground = texture; }

		void OnImGuiRender();
		std::string Compile();

	private:
		ImColor GetIconColor(PinType type);
		void DrawPinIcon(const Pin& pin, bool connected, int alpha);

		void HandleInteraction();

		void RecursiveOptionsMenu(const std::vector<Node<CreateOptions>*>& children);
		void RecursiveSearch(const ShaderNode* currentNode, StringBuilder& shaderCode, StringBuilder& globalCode, std::unordered_set<uint64_t>* nodeTracking);

		Pin* FindPin(ImNode::PinId id);
		Link* FindPinLink(ImNode::PinId id);
		bool IsPinLinked(ImNode::PinId id);

		//This has to be called every time a node is added because the node pointers inside of the pins will be corrupted
		void BuildNodes();

		template<class CustomNode>
		void RegisterNodeType(std::string category) {
			size_t pos = category.find("/");
			Node<CreateOptions>* currentNode = m_CreateOptions.GetRoot();

			while(pos != std::string::npos) {
				std::string subCategory = category.substr(0, pos);
				category = category.substr(pos + 1);

				//Create subcategory if it doesn't exist
				currentNode = currentNode->AddChild({ subCategory });
				pos = category.find("/");
			}

			if (!category.empty())
				currentNode = currentNode->AddChild({ category });

			m_CreateOptions.AddChild(currentNode, { CustomNode::s_Name, []() { return CreateRef<CustomNode>(); } });
		}

	private:
		//Added in the constructor
		Ref<Texture2D> m_HeaderBackground;

		std::string m_Type;
		ImNode::EditorContext* m_Context = nullptr;

		std::vector<Ref<ShaderNode>> m_Nodes;
		ImVector<Link> m_Links;

		NaryTree<CreateOptions> m_CreateOptions;

		int m_NextLinkId = 100;
	};
}
