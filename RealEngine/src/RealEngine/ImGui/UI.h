#pragma once
#include <cstdint>

#include <imgui.h>

//This just adds functionality to the ImGui library
namespace ImGui {
	bool DragDouble(const char* label, double* v, float v_speed = 1.0f, double v_min = 0, double v_max = 0, const char* format = NULL, ImGuiSliderFlags flags = 0) {
		return DragScalar(label, ImGuiDataType_Double, v, v_speed, &v_min, &v_max, format, flags);
	}
	
	bool DragU32Int(const char* label, uint32_t* v, float v_speed = 1.0f, uint32_t v_min = 0, uint32_t v_max = 0, const char* format = NULL, ImGuiSliderFlags flags = 0) {
		return DragScalar(label, ImGuiDataType_U32, v, v_speed, &v_min, &v_max, format, flags);
	}
}

namespace RealEngine::UI {
	struct ScopedStyleColor {
		ScopedStyleColor() = default;

		ScopedStyleColor(ImGuiCol idx, ImVec4 color, bool predicate = true)
			: m_Set(predicate) {
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true)
			: m_Set(predicate) {
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		~ScopedStyleColor() {
			if (m_Set)
				ImGui::PopStyleColor();
		}
	private:
		bool m_Set = false;
	};
}
