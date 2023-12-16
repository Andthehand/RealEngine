#include "ShaderPanelManager.h"

#include "ShaderPanelSerializer.h"

#include <fstream>

namespace RealEngine {
	ShaderPanelManager::ShaderPanelManager() {
		m_HeaderBackground = Texture2D::Create("Resources/Icons/ShaderCreate/BlueprintHeader.png");

		ShaderPanel::RegisterNodeTypes();

		//m_ShaderPanels[ShaderType::Vertex] = CreateRef<ShaderPanel>("Vertex");
		//m_ShaderPanels[ShaderType::Vertex]->SetHeaderBackground(m_HeaderBackground);
		//
		//m_ShaderPanels[ShaderType::Fragment] = CreateRef<ShaderPanel>("Fragment");
		//m_ShaderPanels[ShaderType::Fragment]->SetHeaderBackground(m_HeaderBackground);

		ShaderPanelSerializer::Deserialize("Combined.shaderpanel", m_ShaderPanels);
		m_ShaderPanels[ShaderType::Vertex]->SetHeaderBackground(m_HeaderBackground);
		m_ShaderPanels[ShaderType::Fragment]->SetHeaderBackground(m_HeaderBackground);
	}
	
	ShaderPanelManager::~ShaderPanelManager() {
		ShaderPanelSerializer::Serialize(m_ShaderPanels, "Combined.shaderpanel");
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
	
	void ShaderPanelManager::PreProcess(std::filesystem::path& ubershader, std::string shaders[2], CompileData shaderData[2]) {
		std::string ubershaderSource = ReadFile(ubershader);
		
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = ubershaderSource.find(typeToken, 0); //Start of shader type declaration line
		int shaderIndex = 0;
		while (pos != std::string::npos) {
			size_t eol = ubershaderSource.find_first_of("\r\n", pos); //End of shader type declaration line
			RE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = ubershaderSource.substr(begin, eol - begin);

			size_t nextLinePos = ubershaderSource.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			RE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = ubershaderSource.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaders[shaderIndex] = (pos == std::string::npos) ? ubershaderSource.substr(nextLinePos) : ubershaderSource.substr(nextLinePos, pos - nextLinePos);

			size_t pos = shaders[shaderIndex].find("#GlobalCustomCode");
			shaders[shaderIndex].replace(pos, 17, shaderData[shaderIndex].ShaderGlobalCode);
			pos = shaders[shaderIndex].find("#CustomCode", pos);
			shaders[shaderIndex].replace(pos, 11, shaderData[shaderIndex].ShaderCode);
			shaderIndex++;
		}
	}

	void ShaderPanelManager::Compile() {
		std::string shaders[2];
		CompileData shaderData[2];
		
		shaderData[0] = m_ShaderPanels[ShaderType::Vertex]->Compile();
		shaderData[1] = m_ShaderPanels[ShaderType::Fragment]->Compile();
		PreProcess((std::filesystem::path)"assets/shaders/UberShader.glsl", shaders, shaderData);

		RE_CORE_WARN("ShaderCode:\n{0}", shaders[0]);
		RE_CORE_WARN("ShaderCode:\n{0}", shaders[1]);

		//Combine vector and fragment defines together
		std::vector<std::string> defines;
		defines.reserve(shaderData[0].ShaderDefines.size() + shaderData[1].ShaderDefines.size());
		defines.insert(defines.end(), shaderData[0].ShaderDefines.begin(), shaderData[0].ShaderDefines.end());
		defines.insert(defines.end(), shaderData[1].ShaderDefines.begin(), shaderData[1].ShaderDefines.end());

		m_PreviewShader = Shader::Create("Preview Shader", shaders[0], shaders[1], defines, &m_Reflect);

		//for (auto& shaderCode : m_Reflect.ShaderCode)
		//	RE_CORE_WARN("ShaderCode:\n{0}", shaderCode);
	}
}
