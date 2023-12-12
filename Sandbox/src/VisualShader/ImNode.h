#pragma once
#include <RealEngine.h>

#include <imgui_node_editor.h>

#include "../Utils/builders.h"
#include "../Utils/widgets.h"

namespace ImNode = ax::NodeEditor;
using PinKind = ax::NodeEditor::PinKind;
using ax::Widgets::IconType;

namespace RealEngine {
	class UniqueId {
	public:
		static inline void SetNextID(int id) {
			s_UniqueId = id;
		}

		static inline int GetNextId() {
			if (s_UniqueId == INT_MAX)
				s_UniqueId = 0;

			return s_UniqueId++;
		}
	public:
		static inline int s_UniqueId = 1;
	};
}

namespace ax::NodeEditor {
	static void NodeLabel(const char* label, ImColor color) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
		ImVec2 size = ImGui::CalcTextSize(label);

		ImVec2 padding = ImGui::GetStyle().FramePadding;
		ImVec2 spacing = ImGui::GetStyle().ItemSpacing;

		ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

		ImVec2 rectMin = ImGui::GetCursorScreenPos() - padding;
		ImVec2 rectMax = ImGui::GetCursorScreenPos() + size + padding;

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
		ImGui::TextUnformatted(label);
	};

	static bool DrawNodeControlN(void* p_data, int components, ImGuiDataType dataType, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL) {
		RE_ASSERT(components <= 4, "Cant't create a node greater than a vec4");
		RE_ASSERT(components > 0, "Cant't create a node no components?");
		if (components > 4 || components < 0) {
			return false;
		}

		bool value_changed = false;
		ImGui::BeginGroup();

		//For some reason the compiler doesn't like this
		//ImGui::PushMultiItemsWidths(4, 64 * components);

		float width;
		if (components == 1)
			width = 128 + ImGui::GetStyle().FramePadding.x;
		else
			width = 64;

		for (int i = 0; i < components; i++) {
			static const ImU32 colors[] = {
				0xBB0000FF, // red
				0xBB00FF00, // green
				0xBBFF0000, // blue
				0xBBFFFFFF, // white for alpha?
			};

			ImGui::PushID(i);
			ImGui::SetNextItemWidth(width);
			value_changed |= ImGui::DragScalar("", dataType, p_data, v_speed, p_min, p_max, format);
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

		ImGui::EndGroup();

		return value_changed;
	}

	static bool DrawNodeVec4Control(glm::vec4* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f") {
		return DrawNodeControlN(v, 4, v_speed, ImGuiDataType_Float, &v_min, &v_max, format);
	}

	static bool DrawNodeVec3Control(glm::vec3* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f") {
		return DrawNodeControlN(v, 3, v_speed, ImGuiDataType_Float, &v_min, &v_max, format);
	}

	static bool DrawNodeVec2Control(glm::vec2* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f") {
		return DrawNodeControlN(v, 2, v_speed, ImGuiDataType_Float, &v_min, &v_max, format);
	}
}
