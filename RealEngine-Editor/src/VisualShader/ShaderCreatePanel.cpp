#include "ShaderCreatePanel.h"

//TODO: This might be really bad
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#include "Nodes/ShaderNodes.h"

namespace RealEngine {
	ShaderCreatePanel::ShaderCreatePanel() {
		ImNode::Config config;
		config.SettingsFile = "Simple.json";
		m_Context = ImNode::CreateEditor(&config);

		m_HeaderBackground = RealEngine::Texture2D::Create("Resources/Icons/ShaderCreate/BlueprintHeader.png");

		//Init Testing Nodes
		m_Nodes.emplace_back(CreateRef<FragmentShaderOutputNode>());

		m_Nodes.emplace_back(CreateRef<ShaderTextureNode>());

		BuildNodes();
	}

	ImColor ShaderCreatePanel::GetIconColor(PinType type) {
		switch (type) {
		default:
			case PinType::Bool:     return ImColor(220, 48, 48);
			case PinType::Int:      return ImColor(68, 201, 156);
			case PinType::Float:    return ImColor(147, 226, 74);
			case PinType::String:   return ImColor(124, 21, 153);
		}
	};

	void ShaderCreatePanel::DrawPinIcon(const Pin& pin, bool connected, int alpha) {
		//TODO: Implement!
		IconType iconType;
		ImColor  color = GetIconColor(pin.Type);
		color.Value.w = alpha / 255.0f;
		switch (pin.Type) {
			case PinType::Bool:     iconType = IconType::Diamond; break;
			case PinType::Int:      iconType = IconType::Circle; break;
			case PinType::Float:    iconType = IconType::Circle; break;
			case PinType::Vector2:  iconType = IconType::Circle; break;
			case PinType::Vector3:  iconType = IconType::Circle; break;
			case PinType::String:   iconType = IconType::RoundSquare; break;
			default:
				return;
		}

		//ax::Widgets::Icon(iconType, connected, color, ImColor(32, 32, 32, alpha));
		ax::Widgets::Icon(iconType, connected, color, ImColor(32, 32, 32, alpha));
	};

	void ShaderCreatePanel::OnImGuiRender() {
		ImGui::Begin("Shader Creation Editor");

		ImNode::SetCurrentEditor(m_Context);
		ImNode::Begin("My Editor");

		//You have to suspend ImNode because https://github.com/thedmd/imgui-node-editor/issues/37#issuecomment-549122793
		ImNode::Suspend();
			if (ImNode::ShowBackgroundContextMenu()) {
				ImGui::OpenPopup("Create New Node");
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
			if (ImGui::BeginPopup("Create New Node")) {

				if (ImGui::MenuItem("Compile")) Compile();
				ImGui::EndPopup();
			}
			ImGui::PopStyleVar();
		ImNode::Resume();

		{
			auto cursorTopLeft = ImGui::GetCursorScreenPos();

			ImNode::Utilities::BlueprintNodeBuilder builder((ImTextureID)(uint64_t)m_HeaderBackground->GetRendererID(),
				m_HeaderBackground->GetWidth(), m_HeaderBackground->GetHeight());

			for (auto& node : m_Nodes) {

				//TODO: Implement
				/*if (node.Type != NodeType::Blueprint && node.Type != NodeType::Simple)
					continue;*/

				//const auto isSimple = node.Type == NodeType::Simple;
				const auto isSimple = false;

				builder.Begin(node->ID);
				if (!isSimple) {
					//TODO: Implement
					//builder.Header(node.Color);
					builder.Header();
					ImGui::Spring(0);
					ImGui::TextUnformatted(node->Name.c_str());
					ImGui::Spring(1);
					ImGui::Dummy(ImVec2(0, 28));
					ImGui::Spring(0);
					builder.EndHeader();
				}

				for (auto& input : node->Inputs) {
					auto alpha = ImGui::GetStyle().Alpha;
					//TODO: Implement!
					/*if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);*/

					builder.Input(input.ID);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					//TODO: Implement!
					DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
					ImGui::Spring(0);
					if (!input.Name.empty()) {
						ImGui::TextUnformatted(input.Name.c_str());
						ImGui::Spring(0);
					}
					ImGui::PopStyleVar();
					builder.EndInput();
				}

				for (auto& output : node->Outputs) {
					auto alpha = ImGui::GetStyle().Alpha;
					//TODO: Implement!
					/*if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);*/

					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					builder.Output(output.ID);
					//TODO: Implement!
					/*if (output.Type == PinType::String) {
						static char buffer[128] = "Edit Me\nMultiline!";
						static bool wasActive = false;

						ImGui::PushItemWidth(100.0f);
						ImGui::InputText("##edit", buffer, 127);
						ImGui::PopItemWidth();
						if (ImGui::IsItemActive() && !wasActive) {
							ImNode::EnableShortcuts(false);
							wasActive = true;
						}
						else if (!ImGui::IsItemActive() && wasActive) {
							ImNode::EnableShortcuts(true);
							wasActive = false;
						}
						ImGui::Spring(0);
					}*/
					if (!output.Name.empty()) {
						ImGui::Spring(0);
						ImGui::TextUnformatted(output.Name.c_str());
					}
					ImGui::Spring(0);
					DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
					ImGui::PopStyleVar();
					builder.EndOutput();
				}

				builder.End();
			}
		}

		// Submit Links for drawing
		for (auto& linkInfo : m_Links)
			ImNode::Link(linkInfo.Id, linkInfo.InputPin->ID, linkInfo.OutputPin->ID);

		HandleInteraction();

		// End of interaction with editor.
		ImNode::End();

		ImNode::SetCurrentEditor(nullptr);

		ImGui::End();
	}
	
	void ShaderCreatePanel::HandleInteraction() {
		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ImNode::BeginCreate()) {
			//Helper Lambda
			//TODO: Move to its own function?
			auto showLabel = [](const char* label, ImColor color) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
				auto size = ImGui::CalcTextSize(label);

				auto padding = ImGui::GetStyle().FramePadding;
				auto spacing = ImGui::GetStyle().ItemSpacing;

				ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

				auto rectMin = ImGui::GetCursorScreenPos() - padding;
				auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

				auto drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
				ImGui::TextUnformatted(label);
			};


			ImNode::PinId inputPinId, outputPinId;
			if (ImNode::QueryNewLink(&inputPinId, &outputPinId) 
				// Check that both IDs are valid
				&& (inputPinId && outputPinId)) {

				auto inputPin = FindPin(inputPinId);
				auto outputPin = FindPin(outputPinId);
				
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
					showLabel("Pin Cannot Connect to Same Node", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (inputPin->Kind == outputPin->Kind) {
					//Can't connect 2 inputs or 2 outputs together
					std::string kind = inputPin->Kind == PinKind::Input ? "Inputs" : "Outputs";

					showLabel(("Can't Connect 2 " + kind + " Together").c_str(), ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (inputPin->Type != outputPin->Type) {
					//Can't connect 2 pins of different types together
					showLabel("Can't Connect Pins of Different Types", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (IsPinLinked(inputPinId)) {
					//Inputs can't have 2 connections
					showLabel("Pin Already Connected", ImColor(171, 44, 44, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else {
					//If hovering over valid pin
					showLabel("Connect Pairs", ImColor(44, 171, 44, 180));

					//If mouse released basically
					if (ImNode::AcceptNewItem(ImColor(0, 255, 0), 2.0f)) {
						//Just add the link to the list
						m_Links.push_back({ ImNode::LinkId(m_NextLinkId++), inputPin, outputPin });
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

					m_Links.erase(&link);
				}
			}
		}
		ImNode::EndDelete();
	}

	static const char* PinTypeToString(PinType pinType) {
		switch (pinType) {
			case RealEngine::PinType::Bool:		return "bool ";
			case RealEngine::PinType::Int:		return "int ";
			case RealEngine::PinType::Float:	return "float ";
			case RealEngine::PinType::Vector2:	return "vec2 ";
			case RealEngine::PinType::Vector3:	return "vec3 ";
			case RealEngine::PinType::String:	return "string ";
		}
		RE_CORE_ASSERT(false);
		return nullptr;
	}

	void ShaderCreatePanel::RecursiveSearch(const ShaderNode* currentNode, std::string &shaderCode) {
		int numberOfInputs = (int)currentNode->Inputs.size();

		std::vector<const Link*> connectedLinks;

		//Going down the chain of Nodes until it reaches the end
		for (int i = 0; i < numberOfInputs; i++) {
			const Link* link = FindPinLink(currentNode->Inputs[i].ID);

			//If there actually is a link
			if (link) {
				connectedLinks.push_back(link);
				RecursiveSearch(link->OutputPin->Node, shaderCode);
			}
		}

		if (!connectedLinks.empty()) {
			std::string nodeName = "\t// " + currentNode->Name + "\n";

			shaderCode += nodeName;
			shaderCode += "\t";
			//shaderCode += PinTypeToString(link->OutputPin->Type);
			//shaderCode += link->InputPin->Name + " = " + link->OutputPin->Name + ";\n";
		}

		//Add to the shader code
		for (const Link* link : connectedLinks) {
			shaderCode += "\t";
			shaderCode += PinTypeToString(link->OutputPin->Type);
			shaderCode += link->InputPin->Name + " = " + link->OutputPin->Name + ";\n";
		}
	}

	void ShaderCreatePanel::Compile() {
		//TODO: Turn this into a custom StringBuilder that has a vector of strings and then joins them all together at the end to keep re alocations low
		//Use this as an example https://github.com/timothyqiu/godot/blob/master/core/string/string_builder.h
		std::string shaderCode = "\nvoid fragment() { \n";

		//m_Nodes[0] is always the output node
		RecursiveSearch(m_Nodes[0].get(), shaderCode);

		shaderCode += "}";

		RE_CORE_WARN("{0}", shaderCode);
	}

	Pin* ShaderCreatePanel::FindPin(ImNode::PinId id) {
		for (auto& node : m_Nodes) {
			for (auto& pin : node->Inputs)
				if (pin.ID == id)
					return &pin;

			for (auto& pin : node->Outputs)
				if (pin.ID == id)
					return &pin;
		}

		RE_CORE_CRITICAL("Coundn't find Pin");
		return nullptr;
	}

	Link* ShaderCreatePanel::FindPinLink(ImNode::PinId id) {
		for (auto& link : m_Links)
			if (link.InputPin->ID == id || link.OutputPin->ID == id)
				return &link;

		return nullptr;
	}

	bool ShaderCreatePanel::IsPinLinked(ImNode::PinId id) {
		for (auto& link : m_Links)
			if (link.InputPin->ID == id || link.OutputPin->ID == id)
				return true;

		return false;
	}

	void ShaderCreatePanel::BuildNodes() {
		for (auto& node : m_Nodes)
			node->BuildNode();
	}
}
