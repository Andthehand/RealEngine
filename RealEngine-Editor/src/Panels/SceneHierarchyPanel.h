#pragma once
#include "RealEngine/Core/Core.h"
#include "RealEngine/Core/UUID.h"
#include "RealEngine/Scene/Scene.h"
#include "RealEngine/Scene/Entity.h"
#include "RealEngine/Events/KeyEvent.h"

namespace RealEngine {
	struct ComponentCopyBuffer {
		size_t ComponentID = 0;
		UUID EntityID = UUID(0);
	};

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext;  }
		void SetSelectedEntity(Entity entity);
	private:
		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction);

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity, Relationship& relationship);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}
