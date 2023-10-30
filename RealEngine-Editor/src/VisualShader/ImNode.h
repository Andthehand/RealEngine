#pragma once

#include <imgui_node_editor.h>

#include "../Utils/builders.h"
#include "../Utils/widgets.h"

namespace ImNode = ax::NodeEditor;
using PinKind = ax::NodeEditor::PinKind;
using ax::Widgets::IconType;

namespace RealEngine {
	inline int GetNextId() { 
		static int m_UniqueId = 1;
		return m_UniqueId++;
	}
}
