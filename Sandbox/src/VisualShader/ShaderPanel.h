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
		ImNode::PinId InputPin;
		ImNode::PinId OutputPin;
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

		void SetHeaderBackground(const Ref<Texture2D> texture) { m_HeaderBackground = texture; }

		void OnImGuiRender();
		std::string Compile();

		const std::vector<Ref<ShaderNode>>& GetNodes() const { return m_Nodes; }
		void AddNodeByRegisterTree(std::string category) {
			if(category.empty())
				return;

			size_t pos = category.find("/");
			Node<CreateOptions>* currentNode = s_CreateOptions.GetRoot();

			while (pos != std::string::npos) {
				std::string subCategory = category.substr(0, pos);
				category = category.substr(pos + 1);

				//Get Children
				auto children = currentNode->GetChildren();
				//Check if the subcategory exists
				auto it = std::find_if(children.begin(), children.end(), 
					[&subCategory](const Node<CreateOptions>* node) 
					{ return node->GetData().Name == subCategory; });
				if(it != children.end())
					currentNode = *it;
				else
					return;

				pos = category.find("/");
			}

			if (!category.empty()) {
				//Get Children
				auto children = currentNode->GetChildren();
				//Check if the subcategory exists
				auto it = std::find_if(children.begin(), children.end(),
										[&category](const Node<CreateOptions>* node)
									{ return node->GetData().Name == category; });
				if (it != children.end())
					currentNode = *it;
				else
					return;
			}

			m_Nodes.emplace_back(currentNode->GetData().CreateFunction());
		}
		//This has to be called every time a node is added because the node pointers inside of the pins will be corrupted
		void BuildNodes();

		void AddLink(ImNode::PinId inputPin, ImNode::PinId outputPin);

		static void RegisterNodeTypes();
	private:
		ImColor GetIconColor(PinType type);
		void DrawPinIcon(const Pin& pin, bool connected, int alpha);

		void HandleInteraction();

		void RecursiveOptionsMenu(const std::vector<Node<CreateOptions>*>& children);
		void RecursiveSearch(const ShaderNode* currentNode, StringBuilder& shaderCode, StringBuilder& globalCode, std::unordered_set<uint64_t>* nodeTracking);

		Pin* FindPin(ImNode::PinId id);
		Link* FindPinLink(ImNode::PinId id);
		bool IsPinLinked(ImNode::PinId id);

		template<class CustomNode>
		static void RegisterNodeType() {
			std::string category = std::string(CustomNode::s_OptionPath);
			size_t pos = category.find("/");
			Node<CreateOptions>* currentNode = s_CreateOptions.GetRoot();

			while(pos != std::string::npos) {
				std::string subCategory = category.substr(0, pos);
				category = category.substr(pos + 1);

				//Create subcategory if it doesn't exist
				currentNode = currentNode->AddChild({ subCategory });
				pos = category.find("/");
			}

			if (!category.empty())
				currentNode = currentNode->AddChild({ category });

			s_CreateOptions.AddChild(currentNode, { CustomNode::s_Name, []() { return CreateRef<CustomNode>(); } });
		}

	private:
		//Added in the constructor
		Ref<Texture2D> m_HeaderBackground;

		std::string m_Type;
		ImNode::EditorContext* m_Context = nullptr;

		std::vector<Ref<ShaderNode>> m_Nodes;
		ImVector<Link> m_Links;

		static NaryTree<CreateOptions> s_CreateOptions;

		int m_NextLinkId = 100;
	};
}
