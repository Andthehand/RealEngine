#include "ShaderPanelManager.h"

#include "ShaderPanelSerializer.h"

#include <fstream>

namespace RealEngine {
	ShaderPanelManager::ShaderPanelManager(const std::filesystem::path& path) 
		: Resource(path) {
		m_HeaderBackground = Texture2D::Create("Resources/Icons/ShaderCreate/BlueprintHeader.png");

		ShaderPanel::RegisterNodeTypes();

		//m_ShaderPanels[ShaderType::Vertex] = CreateRef<ShaderPanel>("Vertex");
		//m_ShaderPanels[ShaderType::Vertex]->SetHeaderBackground(m_HeaderBackground);
		//
		//m_ShaderPanels[ShaderType::Fragment] = CreateRef<ShaderPanel>("Fragment");
		//m_ShaderPanels[ShaderType::Fragment]->SetHeaderBackground(m_HeaderBackground);

		ShaderPanelSerializer::Deserialize(path, m_ShaderPanels);
		m_ShaderPanels[ShaderType::Vertex]->SetHeaderBackground(m_HeaderBackground);
		m_ShaderPanels[ShaderType::Fragment]->SetHeaderBackground(m_HeaderBackground);
	}
	
	ShaderPanelManager::~ShaderPanelManager() {
		SaveAndCompile();
	}
	
	void ShaderPanelManager::OnImGuiRender() {
		static bool p_open = true;
		ImGui::Begin("Shader Creation Editor", &p_open, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Options")) {
				if (ImGui::MenuItem("Compile"))
					SaveAndCompile();
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
									(ImGui::GetStyle().FramePadding.x) * 2.0f);

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

	std::string ShaderPanelManager::ReadFile(const std::filesystem::path& filepath) {
		RE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1) {
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else {
				RE_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else {
			RE_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}
	
	std::string ShaderPanelManager::PreProcess(std::filesystem::path& ubershader, CompileData shaderData[2]) {
		std::string ubershaderSource = ReadFile(ubershader);
		size_t pos = 0; //Start of shader type declaration line
		for (size_t i = 0; i < 2; i++) {
			pos = ubershaderSource.find("#CustomDefines");
			ubershaderSource.erase(pos, 14);
			for (auto& define : shaderData[0].ShaderDefines)
				ubershaderSource.insert(pos, "#define " + define + "\n");
			for (auto& define : shaderData[1].ShaderDefines)
				ubershaderSource.insert(pos, "#define " + define + "\n");

			pos = ubershaderSource.find("#GlobalCustomCode");
			ubershaderSource.replace(pos, 17, shaderData[i].ShaderGlobalCode);
			pos = ubershaderSource.find("#CustomCode", pos);
			ubershaderSource.replace(pos, 11, shaderData[i].ShaderCode);
		}

		return ubershaderSource;
	}

	void ShaderPanelManager::SaveAndCompile() {
		std::string shaders[2];
		CompileData shaderData[2];
		
		shaderData[0] = m_ShaderPanels[ShaderType::Vertex]->Compile();
		shaderData[1] = m_ShaderPanels[ShaderType::Fragment]->Compile();
		std::string processedShader = PreProcess(std::filesystem::path("assets/shaders/UberShader.glsl"), shaderData);

		ShaderPanelSerializer::Serialize(m_ShaderPanels, Resource::GetPath().filename(), processedShader);
	}
}
