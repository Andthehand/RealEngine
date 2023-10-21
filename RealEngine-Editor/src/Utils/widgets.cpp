# define IMGUI_DEFINE_MATH_OPERATORS
# include "widgets.h"
# include <imgui_internal.h>

void ax::Widgets::Icon(IconType type, bool filled, const ImVec4& color/* = ImVec4(1, 1, 1, 1)*/, const ImVec4& innerColor/* = ImVec4(0, 0, 0, 0)*/) {
	ImVec2 widgetSize = ImVec2(24.0f, 24.0f);

    if (ImGui::IsRectVisible(widgetSize)) {
        auto cursorPos = ImGui::GetCursorScreenPos();
        auto drawList  = ImGui::GetWindowDrawList();
        ax::Drawing::DrawIcon(drawList, cursorPos, cursorPos + widgetSize, type, filled, ImColor(color), ImColor(innerColor));
    }

    ImGui::Dummy(widgetSize);
}

