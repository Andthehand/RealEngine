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
		Ref<RealEngine::Texture2D> m_Texture;
		Ref<RealEngine::Texture2D> m_SpriteSheet;
		Ref<RealEngine::SubTexture2D> m_GrassTexture;
		Ref<RealEngine::Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };;

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

		//FPS counter
		float deltaTime = 0.0f;
		float fps = 0.0f;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}