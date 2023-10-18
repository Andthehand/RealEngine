#include "ShaderCreatePanel.h"

namespace RealEngine {
	ShaderCreatePanel::ShaderCreatePanel() {
		ImNode::Config config;
		config.SettingsFile = "Simple.json";
		m_Context = ImNode::CreateEditor(&config);
	}

	void ShaderCreatePanel::OnImGuiRender() {
		ImGui::Begin("Shader Creation Editor");

		ImNode::SetCurrentEditor(m_Context);
		ImNode::Begin("My Editor");
		int uniqueId = 1;
		// Start drawing nodes.
		ImNode::BeginNode(uniqueId++);
			ImGui::Text("Node A");
			ImNode::BeginPin(uniqueId++, ImNode::PinKind::Input);
				ImGui::Text("-> In");
			ImNode::EndPin();
				ImGui::SameLine();
			ImNode::BeginPin(uniqueId++, ImNode::PinKind::Output);
				ImGui::Text("Out ->");
			ImNode::EndPin();
		ImNode::EndNode();
		ImNode::End();
		ImNode::SetCurrentEditor(nullptr);

		ImGui::End();
	}
}
