#include "ShaderCreatePanel.h"

//TODO: This might be really bad
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

namespace RealEngine {
	ShaderCreatePanel::ShaderCreatePanel() {
		ImNode::Config config;
		config.SettingsFile = "Simple.json";
		m_Context = ImNode::CreateEditor(&config);

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

	void ShaderCreatePanel::OnImGuiRender() {
		ImGui::Begin("Shader Creation Editor");

		ImNode::SetCurrentEditor(m_Context);
		ImNode::Begin("My Editor");

		//Display all nodes
		for (Node& node : m_Nodes) {
			//Start Node
			ImNode::BeginNode(node.ID);
			ImGui::Text(node.Name.c_str());
				//Display inputs
				for (Pin& input : node.Inputs) {
					ImNode::BeginPin(input.ID, PinKind::Input);
						ImGui::Text(input.Name.c_str());
					ImNode::EndPin();
				}

				ImGui::SameLine();

				//Display inputs
				for (Pin& output : node.Outputs) {
					ImNode::BeginPin(output.ID, PinKind::Output);
						ImGui::Text(output.Name.c_str());
					ImNode::EndPin();
				}
			ImNode::EndNode();
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
					
				if (inputPin->Kind == PinKind::Input) {
					std::swap(inputPin, outputPin);
					std::swap(inputPinId, outputPinId);
				}

				if (inputPin == outputPin) {
					ImNode::RejectNewItem();
				}
				else if (inputPin->Node == outputPin->Node) {
					//Can't connect input and output from same node together
					showLabel("Pin Cannot Connect to Same Node", ImColor(45, 32, 32, 180));
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (inputPin->Kind == outputPin->Kind) {
					//Can't connect 2 inputs or 2 outputs together
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else {
					//If hovering over valid pin
					showLabel("Connect Pairs", ImColor(32, 90, 32, 180));

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

				// You may reject link deletion by calling:
				// ImNode::RejectDeletedItem();
			}
		}
		ImNode::EndDelete();
	}

	Pin* ShaderCreatePanel::FindPin(ImNode::PinId id) {
		if (!id)
			return nullptr;

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
}
