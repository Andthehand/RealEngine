#pragma once
#include "RealEngine.h"
#include "Panels/SceneHierarchyPanel.h"

#include "RealEngine/Renderer/EditorCamera.h"

namespace RealEngine {
	class EditorLayer : public Layer {
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
		void SaveScene();
	private:
		Ref<RealEngine::Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;

		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = -1;

		//FPS counter
		float deltaTime = 0.0f;
		float fps = 0.0f;

		bool m_Wireframe = false;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
