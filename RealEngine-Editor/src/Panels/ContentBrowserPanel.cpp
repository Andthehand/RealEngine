#include "repch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

#include "../Utils/FileFormats.h"

namespace RealEngine {
	// Once we have projects, change this
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath) {
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	static inline const char* GetPayloadType(std::string& str) {
		if (FileExtenstion::DoesExtensionExist(FileExtenstion::STBI_IMAGE_EXTENSTIONS, str)) {
			return "CONTENT_BROWSER_IMAGE";
		}
		else if (FileExtenstion::DoesExtensionExist(FileExtenstion::REALENGINE_SCENE_EXTENTIONS, str)) {
			return "CONTENT_BROWSER_SCENE";
		}
		else {
			return "CONTENT_BROWSER_ITEM";
		}
	}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath)) {
			if (ImGui::Button("<-")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 32.0f;
		static float iconSize = 128.0f;
		float cellSize = iconSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const std::filesystem::path& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.filename().string();
			std::string extensionString = relativePath.extension().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 });

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relativePath.c_str();

				//This is the payload
				ImGui::SetDragDropPayload(GetPayloadType(path.extension().string()), itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));

				//Thumbnail
				ImGui::Image((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				ImGui::EndDragDropSource();
			}
			ImGui::PopStyleVar();

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Icon Size", &iconSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
	}
}
