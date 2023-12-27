#include "MainLayer.h"

#include <imgui.h>

namespace RealEngine {
	MainLayer::MainLayer() : RealEngine::Layer("MainLayer") { }

	void MainLayer::OnAttach() {
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		float vertices[] = {
			//Positions				//UVs
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		}; 

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_UV" }
		});

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, 6);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_Texture->Bind();
	}

	void MainLayer::OnImGuiRender() {
		static bool p_open = true;
		ImGui::ShowMetricsWindow(&p_open);
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		ImGui::End();

		ImGui::Begin("Viewport");

		ImVec2 viewpoerPanelSize = ImGui::GetContentRegionAvail();
		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewpoerPanelSize.x, viewpoerPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		ImGui::End();
	}

	void MainLayer::OnUpdate(Timestep ts) {
		m_Framebuffer->Bind();
		RenderCommand::Clear();

		RenderCommand::DrawIndexed(m_VertexArray, 6);

		m_Framebuffer->UnBind();
	}
}
