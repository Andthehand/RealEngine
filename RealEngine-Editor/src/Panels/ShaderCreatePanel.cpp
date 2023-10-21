#include "ShaderCreatePanel.h"

//TODO: This might be really bad
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#include "../Utils/builders.h"
#include "../Utils/widgets.h"

using ax::Widgets::IconType;

namespace RealEngine {
	ShaderCreatePanel::ShaderCreatePanel() {
		ImNode::Config config;
		config.SettingsFile = "Simple.json";
		m_Context = ImNode::CreateEditor(&config);

		m_HeaderBackground = RealEngine::Texture2D::Create("Resources/Icons/ShaderCreate/BlueprintHeader.png");

		//Init Testing Nodes
		m_Nodes.emplace_back(GetNextId(), "Node A");
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "-> In");
		m_Nodes.back().Outputs.emplace_back(GetNextId(), "Out ->");
		m_Nodes.back().BuildNode();

		m_Nodes.emplace_back(GetNextId(), "Node B");
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "-> In1");
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "-> In2");
		m_Nodes.back().Outputs.emplace_back(GetNextId(), "Out ->");
		m_Nodes.back().BuildNode();
	}

	void ShaderCreatePanel::DrawPinIcon(const Pin& pin, bool connected, int alpha) {
		//TODO: Implement!
		IconType iconType = IconType::Circle;
		ImColor color = ImColor(147, 226, 74);
		/*ImColor  color = GetIconColor(pin.Type);
		color.Value.w = alpha / 255.0f;
		switch (pin.Type) {
			case PinType::Flow:     iconType = IconType::Flow;   break;
			case PinType::Bool:     iconType = IconType::Circle; break;
			case PinType::Int:      iconType = IconType::Circle; break;
			case PinType::Float:    iconType = IconType::Circle; break;
			case PinType::String:   iconType = IconType::Circle; break;
			case PinType::Object:   iconType = IconType::Circle; break;
			case PinType::Function: iconType = IconType::Circle; break;
			case PinType::Delegate: iconType = IconType::Square; break;
			default:
				return;
		}*/

		//ax::Widgets::Icon(iconType, connected, color, ImColor(32, 32, 32, alpha));
		ax::Widgets::Icon(iconType, connected, color, ImColor(32, 32, 32, alpha));
	};

	void ShaderCreatePanel::OnImGuiRender() {
		ImGui::Begin("Shader Creation Editor");

		ImNode::SetCurrentEditor(m_Context);
		ImNode::Begin("My Editor");
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

				builder.Begin(node.ID);
				if (!isSimple) {
					//TODO: Implement
					//builder.Header(node.Color);
					builder.Header();
					ImGui::Spring(0);
					ImGui::TextUnformatted(node.Name.c_str());
					ImGui::Spring(1);
					ImGui::Dummy(ImVec2(0, 28));
					ImGui::Spring(0);
					builder.EndHeader();
				}

				for (auto& input : node.Inputs) {
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

				for (auto& output : node.Outputs) {
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
			ImNode::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

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
				else {
					//If hovering over valid pin
					showLabel("Connect Pairs", ImColor(44, 171, 44, 180));

					//If mouse released basically
					if (ImNode::AcceptNewItem(ImColor(0, 255, 0), 2.0f)) {
						m_Links.push_back({ ImNode::LinkId(m_NextLinkId++), inputPinId, outputPinId });

						// Draw new link.
						ImNode::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
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
				if (ImNode::AcceptDeletedItem()) {
					// Then remove link from your data.
					for (auto& link : m_Links) {
						if (link.Id == deletedLinkId) {
							m_Links.erase(&link);
							break;
						}
					}
				}
			}
		}
		ImNode::EndDelete();
	}

	Pin* ShaderCreatePanel::FindPin(ImNode::PinId id) {
		for (auto& node : m_Nodes) {
			for (auto& pin : node.Inputs)
				if (pin.ID == id)
					return &pin;

			for (auto& pin : node.Outputs)
				if (pin.ID == id)
					return &pin;
		}

		return nullptr;
	}

	bool ShaderCreatePanel::IsPinLinked(ImNode::PinId id) {
		for (auto& link : m_Links)
			if (link.InputId == id || link.OutputId == id)
				return true;

		return false;
	}
}
