#include "ShaderCreatePanel.h"

namespace RealEngine {
	ShaderCreatePanel::ShaderCreatePanel() {
		ImNode::Config config;
		config.SettingsFile = "Simple.json";
		m_Context = ImNode::CreateEditor(&config);

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
					ImNode::BeginPin(input.ID, ImNode::PinKind::Input);
						ImGui::Text(input.Name.c_str());
					ImNode::EndPin();
				}

				ImGui::SameLine();

				//Display inputs
				for (Pin& output : node.Outputs) {
					ImNode::BeginPin(output.ID, ImNode::PinKind::Output);
						ImGui::Text(output.Name.c_str());
					ImNode::EndPin();
				}
			ImNode::EndNode();
		}

		// Submit Links
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
			ImNode::PinId inputPinId, outputPinId;
			if (ImNode::QueryNewLink(&inputPinId, &outputPinId)) {
				// both are valid, let's accept link
				if (inputPinId && outputPinId) {
					auto inputPin = FindPin(inputPinId);
					auto outputPin = FindPin(outputPinId);
					
					if (inputPin->Kind == ImNode::PinKind::Input) {
						std::swap(inputPin, outputPin);
						std::swap(inputPinId, outputPinId);
					}

					if (inputPin == outputPin) {
						ImNode::RejectNewItem();
					}
					// ImNode::AcceptNewItem() return true when user release mouse button.
					else if (ImNode::AcceptNewItem()) {
						// Since we accepted new link, lets add one to our list of links.
						m_Links.push_back({ ImNode::LinkId(m_NextLinkId++), inputPinId, outputPinId });

						// Draw new link.
						ImNode::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
					}
					else if (inputPin->Kind == outputPin->Kind) {
						//Can't connect 2 inputs or 2 outputs together
						ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
					}
					else {
						//If hovering over valid pin
						ImNode::AcceptNewItem(ImColor(0, 255, 0), 2.0f);
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
