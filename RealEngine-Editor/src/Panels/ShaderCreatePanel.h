#pragma once
#include <imgui_node_editor.h>

namespace ImNode = ax::NodeEditor;
namespace RealEngine {
	class ShaderCreatePanel {
	public:
		ShaderCreatePanel();

		void OnImGuiRender();

	private:
		ImNode::EditorContext* m_Context = nullptr;
	};
}
