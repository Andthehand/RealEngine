#pragma once
#include <RealEngine.h>

#include <imgui_node_editor.h>
#include <imgui_internal.h>

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

	static float CalcMaxPopupHeightFromItemCount(int items_count) {
		ImGuiContext& g = *GImGui;
		if (items_count <= 0)
			return FLT_MAX;
		return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
	}

	static bool BeginComboNodePopup(ImGuiID popup_id, ImGuiComboFlags flags) {
		Suspend();

		ImGuiContext& g = *GImGui;
		if (!ImGui::IsPopupOpen(popup_id, ImGuiPopupFlags_None))
		{
			g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
			return false;
		}

		char name[20];
		if (flags & ImGuiWindowFlags_ChildMenu)
			ImFormatString(name, IM_ARRAYSIZE(name), "##Menu_%02d", g.BeginMenuCount); // Recycle windows based on depth
		else
			ImFormatString(name, IM_ARRAYSIZE(name), "##Popup_%08x", popup_id); // Not recycling, so we can close/open during the same frame

		flags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;

		ImGui::SetNextWindowPos(ImGui::GetMousePosOnOpeningCurrentPopup(), ImGuiCond_Appearing);
		bool is_open = ImGui::Begin(name, NULL, flags);

		if (!is_open) // NB: Begin can return false when the popup is completely clipped (e.g. zero size display)
			ImGui::EndPopup();

		return is_open;
	}

	static bool BeginNodeCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0) {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
		g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
		if (window->SkipItems)
			return false;

		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

		const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : ImGui::GetFrameHeight();
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : ImGui::CalcItemWidth();
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
		const ImRect total_bb(bb.Min, bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id, &bb))
			return false;

		// Open on click
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
		const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
		bool popup_open = ImGui::IsPopupOpen(popup_id, ImGuiPopupFlags_None);
		if (pressed && !popup_open)
		{
			ImGui::OpenPopupEx(popup_id, ImGuiPopupFlags_None);
			popup_open = true;
		}

		// Render shape
		const ImU32 frame_col = ImGui::GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
		const float value_x2 = ImMax(bb.Min.x, bb.Max.x - arrow_size);
		ImGui::RenderNavHighlight(bb, id);
		if (!(flags & ImGuiComboFlags_NoPreview))
			window->DrawList->AddRectFilled(bb.Min, ImVec2(value_x2, bb.Max.y), frame_col, style.FrameRounding, (flags & ImGuiComboFlags_NoArrowButton) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersLeft);
		if (!(flags & ImGuiComboFlags_NoArrowButton))
		{
			ImU32 bg_col = ImGui::GetColorU32((popup_open || hovered) ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
			ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
			window->DrawList->AddRectFilled(ImVec2(value_x2, bb.Min.y), bb.Max, bg_col, style.FrameRounding, (w <= arrow_size) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersRight);
			if (value_x2 + arrow_size - style.FramePadding.x <= bb.Max.x)
				ImGui::RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y, bb.Min.y + style.FramePadding.y), text_col, ImGuiDir_Down, 1.0f);
		}
		ImGui::RenderFrameBorder(bb.Min, bb.Max, style.FrameRounding);

		// Custom preview
		if (flags & ImGuiComboFlags_CustomPreview)
		{
			g.ComboPreviewData.PreviewRect = ImRect(bb.Min.x, bb.Min.y, value_x2, bb.Max.y);
			IM_ASSERT(preview_value == NULL || preview_value[0] == 0);
			preview_value = NULL;
		}

		// Render preview and label
		if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
		{
			if (g.LogEnabled)
				ImGui::LogSetNextTextDecoration("{", "}");
			ImGui::RenderTextClipped(bb.Min + style.FramePadding, ImVec2(value_x2, bb.Max.y), preview_value, NULL, NULL);
		}
		if (label_size.x > 0)
			ImGui::RenderText(ImVec2(bb.Max.x + style.ItemInnerSpacing.x, bb.Min.y + style.FramePadding.y), label);

		if (!popup_open)
			return false;

		g.NextWindowData.Flags = backup_next_window_data_flags;

		return BeginComboNodePopup(popup_id, flags);
	}

	static void EndNodeCombo() {
		ImGui::EndPopup();

		Resume();
	}
}
