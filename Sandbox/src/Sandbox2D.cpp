#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) { }

void Sandbox2D::OnAttach() {
	RE_PROFILE_FUNCTION();

	m_Texture = RealEngine::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = RealEngine::Texture2D::Create("assets/textures/Spritesheet.png");
	m_GrassTexture = RealEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 2 }, { 16, 16 });

    RealEngine::FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height= 720;
    m_Framebuffer = RealEngine::Framebuffer::Create(fbSpec);
}

void Sandbox2D::OnDetach() {
	RE_PROFILE_FUNCTION();
}

void Sandbox2D::OnImGuiRender() {
	RE_PROFILE_FUNCTION();

    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
    bool opt_fullscreen = opt_fullscreen_persistant;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
        window_flags |= ImGuiWindowFlags_NoBackground;


    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Dockspace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // Disabling fullscreen would allow the window to be moved to the front of other windows, 
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit")) RealEngine::Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Settings");

    auto stats = RealEngine::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("DrawCalls: %d", stats.DrawCalls);
    ImGui::Text("QuadCount: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::Text("FPS: %f", fps);

    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });
    ImGui::End();

    ImGui::End();
}

void Sandbox2D::OnUpdate(RealEngine::Timestep ts) {
	RE_PROFILE_FUNCTION();

	deltaTime += ts;
	deltaTime /= 2.0f;
	fps = 1.0f / deltaTime;

	//Update
	{
		RE_PROFILE_SCOPE("Camera Update");
		m_CameraController.OnUpdate(ts);
	}
	
	RealEngine::Renderer2D::ResetStats();
	//Render
	{
		RE_PROFILE_SCOPE("Render Prep");
        m_Framebuffer->Bind();
        RealEngine::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		RealEngine::RenderCommand::Clear();
	}

	//Draw
	{
		RE_PROFILE_SCOPE("Render Draw");
		RealEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//for (int x = -5; x < 5; x++) {
		//	for (int y = -5; y < 5; y++) {
		//		glm::vec2 position = { x + (x * 0.1f), y + (y * 0.1f) };
		//		RealEngine::Renderer2D::DrawQuad({ position.x + (x * 0.1f), position.y + (y * 0.1f), 0.0f }, { 1.0f, 1.0f }, {m_SquareColor, 1.0f});
		//	}
		//}

		//RealEngine::Renderer2D::DrawQuad({ 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f }, m_Texture);
		//RealEngine::Renderer2D::DrawRotatedQuad({ -0.5f, 1.0f, 1.0f }, { 0.5f, 0.5f }, 45.0f, m_Texture);

		RealEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_GrassTexture);

		RealEngine::Renderer2D::EndScene();
        m_Framebuffer->UnBind();
	}
}

void Sandbox2D::OnEvent(RealEngine::Event& e) {
	m_CameraController.OnEvent(e);
}