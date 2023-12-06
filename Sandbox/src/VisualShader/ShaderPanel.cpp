#include "ShaderPanel.h"

//TODO: This might be really bad
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Nodes/ShaderNodes.h"

namespace RealEngine {
	ShaderPanel::ShaderPanel(const char* type)
		: m_CreateOptions(CreateOptions("Root")) {
		ImNode::Config config;
		config.SettingsFile = "Simple.json";
		m_Context = ImNode::CreateEditor(&config);

		RegisterNodeType<ShaderTextureNode>("Textures");
		
		RegisterNodeType<ShaderConstantVec4Node>("Vectors/Constants");
		RegisterNodeType<ShaderConstantVec3Node>("Vectors/Constants");
		RegisterNodeType<ShaderConstantVec2Node>("Vectors/Constants");
		
		RegisterNodeType<ShaderConstantFloatNode>("Scalar/Constants");

		//Init Testing Nodes
		if (type == "Vertex")
			m_Nodes.emplace_back(CreateRef<VertexShaderOutputNode>());
		else if (type == "Fragment")
			m_Nodes.emplace_back(CreateRef<FragmentShaderOutputNode>());
		else
			RE_CORE_ASSERT(false, "Shader Type not supported");

		m_Nodes.emplace_back(CreateRef<ShaderTextureNode>());

		BuildNodes();
	}

	static const bool IsPinTypeConvertableTo(PinType pinType1, PinType pinType2) {
		switch (pinType1) {
			case PinType::Bool:
			case PinType::Int:
			case PinType::Float:
			case PinType::Vector2:
			case PinType::Vector3:
			case PinType::Vector4:		
				//This is disgusting
				switch (pinType2) {
					case PinType::Bool:
					case PinType::Int:
					case PinType::Float:
					case PinType::Vector2:
					case PinType::Vector3:
					case PinType::Vector4:
						return true;
					case PinType::Sampler2D:
						return false;
				}
				break;
			case PinType::Sampler2D:	
				return false;
		}
		RE_CORE_ASSERT(false);
		return nullptr;
	}

	static const char* PinTypeToString(PinType pinType) {
		switch (pinType) {
			case PinType::Bool:			return "Boolean";
			case PinType::Int:			return "Integer";
			case PinType::Float:		return "Float";
			case PinType::Vector2:		return "Vector 2";
			case PinType::Vector3:		return "Vector 3";
			case PinType::Vector4:		return "Vector 4";
			case PinType::Sampler2D:	return "Sampler 2D";
		}
		RE_CORE_ASSERT(false);
		return nullptr;
	}

	static const char* PinTypeToCodeType(PinType pinType) {
		switch (pinType) {
			case PinType::Bool:			return "bool ";
			case PinType::Int:			return "int ";
			case PinType::Float:		return "float ";
			case PinType::Vector2:		return "vec2 ";
			case PinType::Vector3:		return "vec3 ";
			case PinType::Vector4:		return "vec4 ";
			case PinType::Sampler2D:	return "sampler2D ";
		}
		RE_CORE_ASSERT(false);
		return nullptr;
	}

	ImColor ShaderPanel::GetIconColor(PinType type) {
		switch (type) {
			case PinType::Bool:			return ImColor(220, 48, 48);
			case PinType::Int:			return ImColor(68, 201, 156);
			case PinType::Float:		return ImColor(97, 217, 245);
			case PinType::Vector2:		return ImColor(189, 145, 242);
			case PinType::Vector3:		return ImColor(214, 125, 237);
			case PinType::Vector4:		return ImColor(255, 32, 242);
			case PinType::Sampler2D:	return ImColor(255, 255, 0);
		}

		//RE_CORE_ASSERT(false);
		return ImColor(255, 255, 255);
	};

	void ShaderPanel::DrawPinIcon(const Pin& pin, bool connected, int alpha) {
		//TODO: Implement!
		IconType iconType;
		ImColor  color = GetIconColor(pin.Type);
		color.Value.w = alpha / 255.0f;
		switch (pin.Type) {
			case PinType::Bool:     
				iconType = IconType::Diamond; break;
			case PinType::Int:      
			case PinType::Float:    
			case PinType::Vector2:  
			case PinType::Vector3:  
			case PinType::Vector4:  
				iconType = IconType::Circle; break;
			case PinType::Sampler2D:
				iconType = IconType::RoundSquare; break;
			default:
				RE_CORE_ASSERT(false);
				return;
		}

		//ax::Widgets::Icon(iconType, connected, color, ImColor(32, 32, 32, alpha));
		ax::Widgets::Icon(iconType, connected, color, ImColor(32, 32, 32, alpha));
	};

	void ShaderPanel::RecursiveOptionsMenu(const std::vector<Node<CreateOptions>*>& children) {
		for (const Node<CreateOptions>* child : children) {
			CreateOptions options = child->GetData();
			if (options.CreateFunction != nullptr) {
				if (ImGui::MenuItem(options.Name.c_str())) {
					m_Nodes.emplace_back(options.CreateFunction());
					BuildNodes();
				}
			}
			else {
				if (ImGui::TreeNode(options.Name.c_str())) {
					RecursiveOptionsMenu(child->GetChildren());
					ImGui::TreePop();
				}
			}
		}
	}

	void ShaderPanel::OnImGuiRender() {
		//Used for rendering tooltip outside of the ImNode::Begin() and ImNode::End()
		std::string tooltip;

		//GUI Setup
		{
			ImNode::SetCurrentEditor(m_Context);
			ImNode::Begin("My Editor");

			//You have to suspend ImNode because https://github.com/thedmd/imgui-node-editor/issues/37#issuecomment-549122793
			ImNode::Suspend();

			if (ImNode::ShowBackgroundContextMenu()) {
				ImGui::OpenPopup("Create New Node");
			}

			if (ImGui::BeginPopup("Create New Node")) {
				RecursiveOptionsMenu(m_CreateOptions.GetRoot()->GetChildren());
				ImGui::EndPopup();
			}
			ImNode::Resume();
		}

		//Rendering of Nodes and lines
		{
			ImNode::Utilities::BlueprintNodeBuilder builder((ImTextureID)(uint64_t)m_HeaderBackground->GetRendererID(),
				m_HeaderBackground->GetWidth(), m_HeaderBackground->GetHeight());

			for (Ref<ShaderNode> node : m_Nodes) {

				//TODO: Implement
				/*if (node.Type != NodeType::Blueprint && node.Type != NodeType::Simple)
					continue;*/

				//const bool isSimple = node.Type == NodeType::Simple;
				const bool isSimple = false;

				builder.Begin(node->ID);
				if (!isSimple) {
					//TODO: Implement
					//builder.Header(node.Color);
					builder.Header();
					ImGui::Spring(0);
					ImGui::TextUnformatted(node->GetName());
					ImGui::Spring(1);
					ImGui::Dummy(ImVec2(0, 28));
					ImGui::Spring(0);
					builder.EndHeader();
				}

				for (Pin& input : node->Inputs) {
					float alpha = ImGui::GetStyle().Alpha;
					//TODO: Implement!
					/*if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);*/

					builder.Input(input.ID);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					//TODO: Implement!
					DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));

					if (ImGui::IsItemHovered())
						tooltip = PinTypeToString(input.Type);

					ImGui::Spring(0);
					if (!input.Name.empty()) {
						ImGui::TextUnformatted(input.Name.c_str());						
						ImGui::Spring(0);
					}
					ImGui::PopStyleVar();
					builder.EndInput();
				}

				for (Pin& output : node->Outputs) {
					float alpha = ImGui::GetStyle().Alpha;
					//TODO: Implement!
					/*if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);*/

					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					builder.Output(output.ID);
					static glm::vec4 test = glm::vec4(1.0f);

					ShaderNodeConstant* constantNode = dynamic_cast<ShaderNodeConstant*>(output.Node);
					if (constantNode != nullptr) {
						switch (output.Type) {
							case PinType::Vector4:
								ImNode::DrawNodeVec4Control((glm::vec4*)constantNode->GetConstant());
								break;
							case PinType::Vector3:
								ImNode::DrawNodeVec3Control((glm::vec3*)constantNode->GetConstant());
								break;
							case PinType::Vector2:
								ImNode::DrawNodeVec2Control((glm::vec2*)constantNode->GetConstant());
								break;
							case PinType::Float:
								ImNode::DrawNodeControlN((float*)constantNode->GetConstant(), 1);
								break;
							default:
								RE_ASSERT(false, "Constant pin type not implemented");
								break;
						}
					}

					if (!output.Name.empty()) {
						ImGui::Spring(0);
						ImGui::TextUnformatted(output.Name.c_str());
					}
					ImGui::Spring(0);
					DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
					if (ImGui::IsItemHovered())
						tooltip = PinTypeToString(output.Type);

					ImGui::PopStyleVar();
					builder.EndOutput();
				}

				builder.End();
			}
		}

		// Submit Links for drawing
		for (Link& linkInfo : m_Links)
			ImNode::Link(linkInfo.Id, linkInfo.InputPin->ID, linkInfo.OutputPin->ID);

		HandleInteraction();

		// End of interaction with editor.
		ImNode::End();

		ImNode::SetCurrentEditor(nullptr);

		//Has to be outside of the ImNode::Begin() and ImNode::End()
		if (!tooltip.empty()) {
			static ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];
			ImGui::PushFont(boldFont);
			ImGui::SetTooltip(tooltip.c_str());
			ImGui::PopFont();
			
			tooltip.clear();
		}
	}
	
	void ShaderPanel::HandleInteraction() {
		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ImNode::BeginCreate()) {
			ImNode::PinId inputPinId, outputPinId;
			if (ImNode::QueryNewLink(&inputPinId, &outputPinId) 
				// Check that both IDs are valid
				&& (inputPinId && outputPinId)) {

				Pin* inputPin = FindPin(inputPinId);
				Pin* outputPin = FindPin(outputPinId);
				
				//Make sure the input and output are actually in the correct orientation
				if (inputPin->Kind == PinKind::Output) {
					std::swap(inputPin, outputPin);
					std::swap(inputPinId, outputPinId);
				}

				if (inputPin == outputPin) {
					ImNode::RejectNewItem();
				}
				else if (inputPin->Node == outputPin->Node) {
					//Can't connect input and output from same node together
					ImNode::NodeLabel("Pin Cannot Connect to Same Node", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (inputPin->Kind == outputPin->Kind) {
					//Can't connect 2 inputs or 2 outputs together
					std::string kind = inputPin->Kind == PinKind::Input ? "Inputs" : "Outputs";

					ImNode::NodeLabel(("Can't Connect 2 " + kind + " Together").c_str(), ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (!IsPinTypeConvertableTo(inputPin->Type, outputPin->Type)) {
					//Can't connect 2 pins of different types together
					ImNode::NodeLabel("Pin Types are Not Compatible", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (IsPinLinked(inputPinId)) {
					//Inputs can't have 2 connections
					ImNode::NodeLabel("Pin Already Connected", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else {
					//If hovering over valid pin
					ImNode::NodeLabel("Connect Pairs", ImColor(44, 171, 44, 180));

					//If mouse released basically
					if (ImNode::AcceptNewItem(ImColor(0, 255, 0), 2.0f)) {
						//Just add the link to the list
						m_Links.push_back({ ImNode::LinkId(m_NextLinkId++), inputPin, outputPin });

						inputPin->ConnectedPin = outputPin;
						outputPin->ConnectedPin = inputPin;
					}
				}
			}
		}

		ImNode::EndCreate(); // Wraps up object creation action handling.


		// Handle deletion action
		if (ImNode::BeginDelete()) {
			// There may be many links marked for deletion, let's loop over them.
			ImNode::LinkId deletedLinkId;
			while (ImNode::QueryDeletedLink(&deletedLinkId)) {
				// If you agree that link can be deleted, accept deletion.
				if (!ImNode::AcceptDeletedItem())
					continue;

				// Then remove link from your data.
				for (Link& link : m_Links) {
					if (link.Id != deletedLinkId)
						continue;

					//This is so scuffed
					//It is now less scuffed
					//and finally now it's way less scuffed
					link.InputPin->ConnectedPin = nullptr;
					link.OutputPin->ConnectedPin = nullptr;

					m_Links.erase(&link);
				}
			}
		}
		ImNode::EndDelete();
	}

	void ShaderPanel::RecursiveSearch(const ShaderNode* currentNode, StringBuilder& shaderCode, StringBuilder& globalCode, std::unordered_set<uint64_t>* nodeTracking) {
		//Going down the chain of Nodes until it reaches the end
		for (const Pin& inputPin: currentNode->Inputs) {
			//If the Node isn't connected continue
			if (inputPin.ConnectedPin == nullptr)
				continue;

			//If connected node hasn't been visited yet then visit it
			const uint64_t nextNodeID = (uint64_t)inputPin.ConnectedPin->Node->ID;
			if(nodeTracking->find(nextNodeID) == nodeTracking->end()) {
				nodeTracking->insert(nextNodeID);
				RecursiveSearch(inputPin.ConnectedPin->Node, shaderCode, globalCode, nodeTracking);
			}
		}

		//Reserve space for the input and output variables
		std::vector<std::string> inputVars;
		inputVars.resize(currentNode->Inputs.size());
		std::string* inputs = inputVars.data();

		std::vector<std::string> outputVars;
		outputVars.resize(currentNode->Outputs.size());
		std::string* outputs = outputVars.data();

		for (int i = 0; i < currentNode->Outputs.size(); i++) {
			if(currentNode->Outputs[i].ConnectedPin == nullptr)
				continue;

			std::string varName;
			varName = "out_";
			varName += std::to_string(currentNode->ID.Get());
			varName += "_";
			varName += currentNode->GetName();
			varName += "_";
			varName += currentNode->Outputs[i].Name;
			varName.erase(remove_if(varName.begin(), varName.end(), isspace), varName.end());

			outputs[i] = PinTypeToCodeType(currentNode->Outputs[i].Type) + varName;
		}	

		//Add to the shader code
		for(int i = 0; i < currentNode->Inputs.size(); i++) {
			const Pin* connectedPin = currentNode->Inputs[i].ConnectedPin;

			if(connectedPin == nullptr)
				continue;

			inputs[i] = "out_";
			inputs[i] += std::to_string(connectedPin->Node->ID.Get());
			inputs[i] += "_";
			inputs[i] += connectedPin->Node->GetName();
			inputs[i] += "_";
			inputs[i] += connectedPin->Name;
			inputs[i].erase(remove_if(inputs[i].begin(), inputs[i].end(), isspace), inputs[i].end());

			switch (currentNode->Inputs[i].Type) {
				case PinType::Float: {
					switch (currentNode->Inputs[i].ConnectedPin->Type) {
						case PinType::Vector2: {
							inputs[i] = inputs[i] + ".x";
						} break;
						case PinType::Vector3: {
							inputs[i] = inputs[i] + ".x";
						} break;
						case PinType::Vector4: {
							inputs[i] = inputs[i] + ".x";
						} break;
						default:
							break;
					}
				} break;
				case PinType::Vector2: {
					switch (currentNode->Inputs[i].ConnectedPin->Type) {
						case PinType::Float: {
							inputs[i] = "vec2(" + inputs[i] + ")";
						} break;
						case PinType::Vector3:
						case PinType::Vector4: {
							inputs[i] = "vec2(" + inputs[i] + ".xy)";
						} break;
						default:
							break;
					}
				} break;

				case PinType::Vector3: {
					switch (currentNode->Inputs[i].ConnectedPin->Type) {
						case PinType::Float: {
							inputs[i] = "vec3(" + inputs[i] + ")";
						} break;
						case PinType::Vector2: {
							inputs[i] = "vec3(" + inputs[i] + ", 0.0)";
						} break;
						case PinType::Vector4: {
							inputs[i] = "vec3(" + inputs[i] + ".xyz)";
						} break;
						default:
							break;
					}
				} break;
				case PinType::Vector4: {
					switch (currentNode->Inputs[i].ConnectedPin->Type) {
						case PinType::Float: {
							inputs[i] = "vec4(" + inputs[i] + ")";
						} break;
						case PinType::Vector2: {
							inputs[i] = "vec4(" + inputs[i] + ", 0.0, 0.0)";
						} break;
						case PinType::Vector3: {
							inputs[i] = "vec4(" + inputs[i] + ", 0.0)";
						} break;
						default:
							break;
						}
				} break;
			default:
				break;
			}
		}

		//Variable names are always out_NodeName_PinName
		shaderCode += currentNode->GenerateCode(outputs, inputs);
		globalCode += currentNode->GenerateGlobalCode(inputs);
	}

	std::string ShaderPanel::Compile() {
		StringBuilder shaderCode;
		StringBuilder globalCode;
		globalCode += "\n#version 450 core\n\n";
		shaderCode += "void main() { \n";

		std::unordered_set<uint64_t> nodeTracking;

		//m_Nodes[0] is always the output node
		RecursiveSearch(m_Nodes[0].get(), shaderCode, globalCode, &nodeTracking);

		shaderCode += "}";
		globalCode += "\n";

		RE_CORE_WARN("{0}", globalCode.as_string() + shaderCode.as_string());

		return globalCode.as_string() + shaderCode.as_string();
	}

	Pin* ShaderPanel::FindPin(ImNode::PinId id) {
		for (Ref<ShaderNode>& node : m_Nodes) {
			for (Pin& pin : node->Inputs)
				if (pin.ID == id)
					return &pin;

			for (Pin& pin : node->Outputs)
				if (pin.ID == id)
					return &pin;
		}

		RE_CORE_CRITICAL("Coundn't find Pin");
		return nullptr;
	}

	Link* ShaderPanel::FindPinLink(ImNode::PinId id) {
		for (Link& link : m_Links)
			if (link.InputPin->ID == id || link.OutputPin->ID == id)
				return &link;

		return nullptr;
	}

	bool ShaderPanel::IsPinLinked(ImNode::PinId id) {
		for (Link& link : m_Links)
			if (link.InputPin->ID == id || link.OutputPin->ID == id)
				return true;

		return false;
	}

	void ShaderPanel::BuildNodes() {
		for (Ref<ShaderNode> node : m_Nodes)
			node->BuildNode();
	}
}
