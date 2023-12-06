#include "ShaderPanelManager.h"

namespace RealEngine {
	ShaderPanelManager::ShaderPanelManager() 
		: m_ShaderPanels{ "Vertex", "Fragment" } {
		m_HeaderBackground = Texture2D::Create("Resources/Icons/ShaderCreate/BlueprintHeader.png");

		m_ShaderPanels[ShaderType::Vertex].SetHeaderBackground(m_HeaderBackground);
		m_ShaderPanels[ShaderType::Fragment].SetHeaderBackground(m_HeaderBackground);
	}
	
	ShaderPanelManager::~ShaderPanelManager() {
		
	}
	
	void ShaderPanelManager::OnImGuiRender() {
		static bool p_open = true;
		ImGui::Begin("Shader Creation Editor", &p_open, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Options")) {
				if (ImGui::MenuItem("Compile"))
					m_QueuedCompile = true;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_ShaderPanels[m_CurrentShaderType].OnImGuiRender();

		//Overlays
		{
			ImGui::SetCursorPosX(16);
			ImGui::SetCursorPosY(92);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);

			//Has to be overridden because the button won't render
			ImGui::PushClipRect(ImVec2(4.0f, 110.0f), ImVec2(2556.0f, 1527.0f), true);

			static const char* items[] = { "Vertex", "Fragment" };
			static int item_current = 0;

			ImGui::SetNextItemWidth(ImGui::CalcTextSize(items[item_current]).x + ImGui::GetFrameHeight() + (ImGui::GetStyle().FramePadding.x) * 2.0);
			if (ImGui::BeginCombo("##combo", items[item_current])) {
				for (int n = 0; n < 2; n++) {
					const bool is_selected = (item_current == n);
					if (ImGui::Selectable(items[n], is_selected)) {
						item_current = n;
						m_CurrentShaderType = (ShaderType)n;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopClipRect();

			ImGui::PopStyleVar();
			//Reset Cursor Position for overlay
			ImGui::SetCursorPosY(ImGui::GetStyle().WindowPadding.y);
			ImGui::SetCursorPosX(ImGui::GetStyle().WindowPadding.x);
		}

		ImGui::End();
	}
	
	void ShaderPanelManager::OnUpdate() {
		if (m_QueuedCompile) {
			Compile();
			m_QueuedCompile = false;
		}
	}
	
	void ShaderPanelManager::Compile() {
		std::string vertexShader = m_ShaderPanels[ShaderType::Vertex].Compile();
		std::string fragmentShader = m_ShaderPanels[ShaderType::Fragment].Compile();

		//Temp
		std::string vertGlobalCode = "#version 450 core\n"
			"in vec3 a_Position;\n"
			"in vec2 a_UV;\n"
			"out vec2 v_UV;\n"
			"void main() {\n"
			"   v_UV = a_UV;\n"
			"   gl_Position = vec4(a_Position, 1.0);\n"
			"}";

		//TODO: Add reflection
		m_PreviewShader = Shader::Create("Preview Shader", vertGlobalCode, fragmentShader);
	}
}
