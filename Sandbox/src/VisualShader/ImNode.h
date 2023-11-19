#pragma once
#include <RealEngine.h>

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

namespace ax::NodeEditor {
	static bool DrawNodeControlN(const std::string& label, void* p_data, int components, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL) {
		if (components > 4) {
			RE_ASSERT(true, "Cant't create a node greater than a vec4");
			return false;
		}

		bool value_changed = false;
		ImGui::BeginGroup();
		ImGui::PushID(label.c_str());

		ImGui::Text(label.c_str());
		ImGui::Spring(0);

		//For some reason the compiler doesn't like this
		//ImGui::PushMultiItemsWidths(4, 64 * components);

		for (int i = 0; i < 4; i++) {
			static const ImU32 colors[] = {
				0xBB0000FF, // red
				0xBB00FF00, // green
				0xBBFF0000, // blue
				0xBBFFFFFF, // white for alpha?
			};

			ImGui::PushID(i);
			ImGui::SetNextItemWidth(64);
			value_changed |= ImGui::DragScalar("", ImGuiDataType_Float, p_data, v_speed, p_min, p_max, format);
			p_data = (void*)((char*)p_data + sizeof(float));

			const ImVec2 min = ImGui::GetItemRectMin();
			const ImVec2 max = ImGui::GetItemRectMax();
			const float spacing = ImGui::GetStyle().FrameRounding;
			const float halfSpacing = spacing / 2;

			// This is the main change
			ImGui::GetWindowDrawList()->AddLine({ min.x + spacing, max.y - halfSpacing }, { max.x - spacing, max.y - halfSpacing }, colors[i], 4);

			ImGui::Spring(0, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::PopID();
			//ImGui::PopItemWidth();
		}

		ImGui::PopID();

		ImGui::EndGroup();

		return value_changed;
	}

	static bool DrawNodeVec4Control(const std::string& label, glm::vec4& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f") {
		return DrawNodeControlN(label, &v, 4, v_speed, &v_min, &v_max, format);
	}

	static bool DrawNodeVec3Control(const std::string& label, glm::vec3& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f") {
		return DrawNodeControlN(label, &v, 3, v_speed, &v_min, &v_max, format);
	}

	static bool DrawNodeVec2Control(const std::string& label, glm::vec2& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f") {
		return DrawNodeControlN(label, &v, 2, v_speed, &v_min, &v_max, format);
	}
}
