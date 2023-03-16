#pragma once
#include "RealEngine/Core/Core.h"
#include "RealEngine/Scene/Scene.h"
#include "RealEngine/Scene/Entity.h"
#include "RealEngine/Events/KeyEvent.h"

namespace RealEngine {
	struct ComponentCopyBuffer {
		size_t ComponentID = 0;
		Entity EntityID;
	};

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
		void OnEvent(Event& event);

		Entity GetSelectedEntity() const { return m_SelectionContext;  }
		void SetSelectedEntity(Entity entity);
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}
