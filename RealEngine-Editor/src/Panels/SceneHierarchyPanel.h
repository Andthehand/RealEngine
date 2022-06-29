#pragma once
#include "RealEngine/Core/Core.h"
#include "RealEngine/Core/Log.h"
#include "RealEngine/Scene/Scene.h"
#include "RealEngine/Scene/Entity.h"

namespace RealEngine {
	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}