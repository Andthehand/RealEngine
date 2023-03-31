#include "repch.h"
#include "Project.h"

#include "ProjectSerializer.h"

namespace RealEngine {
	Ref<Project> Project::New() {
		RE_PROFILE_FUNCTION();
		
		s_ActiveProject = CreateRef<Project>();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path) {
		RE_PROFILE_FUNCTION();
		
		Ref<Project> project = CreateRef<Project>();

		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path)) {
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActive(const std::filesystem::path& path) {
		RE_PROFILE_FUNCTION();
	
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path)) {
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}

		return false;
	}
}
