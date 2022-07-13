#pragma once
#include "RealEngine.h"
#include "Panels/SceneHierarchyPanel.h"

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
	private:
		Ref<RealEngine::Texture2D> m_Texture;
		Ref<RealEngine::Texture2D> m_SpriteSheet;
		Ref<RealEngine::SubTexture2D> m_GrassTexture;
		Ref<RealEngine::Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;

		Entity m_SquareEntity;
		Entity m_CameraEntity, m_SecondCamera;
		bool m_PrimaryCamera = true;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };;

		//FPS counter
		float deltaTime = 0.0f;
		float fps = 0.0f;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
