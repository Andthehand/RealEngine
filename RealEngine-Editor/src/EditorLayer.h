#pragma once
#include "RealEngine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void OnOverlayRender();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();

		void OnDuplicateEntity();

		// UI Panels
		void UI_Toolbar();
	private:
		RealEngine::OrthographicCameraController m_CameraController;

		Ref<RealEngine::Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;

		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = -1;

		bool m_ShowPhysicsColliders = false;

		enum class SceneState {
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;

		//FPS counter
		float deltaTime = 0.0f;
		float fps = 0.0f;

		bool m_Wireframe = false;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		// Editor resources
		Ref<Texture2D> m_IconPlay, m_IconStop;

		Entity m_HoveredEntity;
	};
}
