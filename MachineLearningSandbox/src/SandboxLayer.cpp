#include "SandboxLayer.h"

#include <imgui.h>

#include "implot.h"

#include "MachineLearning.h"

void SandboxLayer::OnUpdate(RealEngine::Timestep ts) {
	RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	RenderCommand::Clear();
}

void SandboxLayer::OnImGuiRender() {
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) Application::Get().Close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	ImGui::Begin("Plots");
	
	std::vector<Point> points = MachineLearning::GetPoints();

	if (ImPlot::BeginPlot("Log Function")) {
		ImPlot::PlotScatter("House Pricing", &points[0].x, &points[0].y, points.size(), 0, 0, (int)(sizeof(float) * (size_t)2));

		{
			static float x[1001], y[1001];
			for (int i = 0; i < 1001; ++i) {
				x[i] = i * 0.01f;
				y[i] = MachineLearning::LogFunction(x[i]);
			}

			ImPlot::PlotLine("f(x)", x, y, 1001);
		}

		ImPlot::EndPlot();
	}

	if (ImPlot::BeginPlot("Error Function")) {
		float tempTheta = MachineLearning::GetTheta()[1];

		static float x[1001], y[1001];
		for (int i = 0; i < 1001; ++i) {
			x[i] = (i - 500) * 0.25f;
			y[i] = MachineLearning::CostFunction({ 0.0f ,x[i] });
		}

		ImPlot::PlotLine("J(x)", x, y, 1001);
		if (ImPlot::BeginLegendPopup("J(x)")) {
			if(ImGui::SliderFloat("Theta", &tempTheta, 0.0f, 20.0f))
				MachineLearning::SetTheta({ 0.0f, tempTheta });
			ImPlot::EndLegendPopup();
		}

		float tempx[1] = { MachineLearning::CostFunction(MachineLearning::GetTheta(), points) };
		float tempy[1] = { MachineLearning::CostFunction({ 0, tempx[0] }) };
		ImPlot::PlotScatter("Error", tempx, tempy, 1);

		ImPlot::EndPlot();

		ImGui::Text("The error of theta: %f", MachineLearning::CostFunction());
	}

	ImGui::End();
}
