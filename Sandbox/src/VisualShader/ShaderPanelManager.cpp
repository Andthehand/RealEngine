#include "ShaderPanelManager.h"

#include "ShaderPanelSerializer.h"

namespace RealEngine {
	ShaderPanelManager::ShaderPanelManager() {
		m_HeaderBackground = Texture2D::Create("Resources/Icons/ShaderCreate/BlueprintHeader.png");

		ShaderPanel::RegisterNodeTypes();

		//m_ShaderPanels[ShaderType::Vertex] = CreateRef<ShaderPanel>("Vertex");
		//m_ShaderPanels[ShaderType::Vertex]->SetHeaderBackground(m_HeaderBackground);
		//
		//m_ShaderPanels[ShaderType::Fragment] = CreateRef<ShaderPanel>("Fragment");
		//m_ShaderPanels[ShaderType::Fragment]->SetHeaderBackground(m_HeaderBackground);

		m_ShaderPanels[ShaderType::Vertex] = ShaderPanelSerializer::Deserialize("Vertex.shaderpanel");
		m_ShaderPanels[ShaderType::Vertex]->SetHeaderBackground(m_HeaderBackground);
		
		m_ShaderPanels[ShaderType::Fragment] = ShaderPanelSerializer::Deserialize("Fragment.shaderpanel");
		m_ShaderPanels[ShaderType::Fragment]->SetHeaderBackground(m_HeaderBackground);
	}
	
	ShaderPanelManager::~ShaderPanelManager() {
		ShaderPanelSerializer::Serialize(m_ShaderPanels[ShaderType::Vertex], "Vertex.shaderpanel");
		ShaderPanelSerializer::Serialize(m_ShaderPanels[ShaderType::Fragment], "Fragment.shaderpanel");
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

		m_ShaderPanels[m_CurrentShaderType]->OnImGuiRender();

		//Overlays
		{
			ImGui::SetCursorPosX(16);
			ImGui::SetCursorPosY(92);
			//Has to be overridden because the button won't render
			ImGui::PushClipRect(ImVec2(4.0f, 110.0f), ImVec2(2556.0f, 1527.0f), true);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);

			static const char* items[] = { "Vertex", "Fragment" };
			static int item_current = 0;

			ImGui::SetNextItemWidth(ImGui::CalcTextSize(items[item_current]).x + 
									ImGui::GetFrameHeight() + 
									(ImGui::GetStyle().FramePadding.x) * 2.0);

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

			ImGui::PopStyleVar();
			ImGui::PopClipRect();
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
		std::string vertexShader = m_ShaderPanels[ShaderType::Vertex]->Compile();
		std::string fragmentShader = m_ShaderPanels[ShaderType::Fragment]->Compile();

		m_PreviewShader = Shader::Create("Preview Shader", vertexShader, fragmentShader, &m_Reflect);

		for (auto& shaderCode : m_Reflect.ShaderCode)
			RE_CORE_WARN("ShaderCode:\n{0}", shaderCode);
	}
}
