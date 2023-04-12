#pragma once

#include <string>
#include <filesystem>

#include "RealEngine/Core/Core.h"

namespace RealEngine {
	struct ProjectConfig {
		std::string Name = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
	};

	class Project {
	public:
		static const std::filesystem::path& GetProjectDirectory() {
			RE_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory() {
			RE_CORE_ASSERT(s_ActiveProject);
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		// TODO(Yan): move to asset manager when we have one
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) {
			RE_CORE_ASSERT(s_ActiveProject);
			return GetAssetDirectory() / path;
		}

		static void SetProjectStartScene(const std::filesystem::path& path) {
			RE_CORE_ASSERT(s_ActiveProject);
			s_ActiveProject->m_Config.StartScene = path;
		}

		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive();
		static bool Save(const std::filesystem::path path);
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};
}
