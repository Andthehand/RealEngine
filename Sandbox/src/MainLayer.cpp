#include "MainLayer.h"

#include <imgui.h>

MainLayer::MainLayer() : RealEngine::Layer("MainLayer"){ }

std::string hex(uint32_t n, uint8_t d) {
	std::string s(d, '0');
	for (int i = d - 1; i >= 0; i--, n >>= 4)
		s[i] = "0123456789ABCDEF"[n & 0xF];
	return s;
};

void MainLayer::DrawCode(int x, int y, int nLines) {
	auto it_a = m_MapAsm.find(m_NES.m_6502.m_Pc);
	int nLineY = (nLines >> 1) * 10 + y;
	if (it_a != m_MapAsm.end()) {
		ImGui::Text((*it_a).second.c_str());
		while (nLineY < (nLines * 10) + y) {
			nLineY += 10;
			if (++it_a != m_MapAsm.end()) {
				ImGui::Text(((*it_a).second).c_str());
			}
		}
	}

	it_a = m_MapAsm.find(m_NES.m_6502.m_Pc);
	nLineY = (nLines >> 1) * 10 + y;
	if (it_a != m_MapAsm.end()) {
		while (nLineY > y) {
			nLineY -= 10;
			if (--it_a != m_MapAsm.end()) {
				ImGui::Text(((*it_a).second).c_str());
			}
		}
	}
}

void MainLayer::OnAttach() {
	RealEngine::Application::Get().GetImGuiLayer()->BlockEvents(false);

	m_Cartridge = RealEngine::CreateRef<Cartridge>((std::filesystem::path)"assets/roms/nestest.nes");
	if (!m_Cartridge->ImageValid())
		RealEngine::Application::Get().Close();

	m_NES.InsertCartridge(m_Cartridge);

	m_MapAsm = m_NES.m_6502.Disassemble(0x0000, 0xFFFF);

	m_NES.Reset();
}

void MainLayer::OnUpdate(RealEngine::Timestep ts) {
	RealEngine::RenderCommand::Clear();

	if (m_EmulationRun) {
		if (m_ResidualTime > 0.0f)
			m_ResidualTime -= ts;
		else {
			m_ResidualTime += (1.0f / 60.0f) - ts;
			do { m_NES.Clock(); } while (!m_NES.m_PPU.m_FrameComplete);
			m_NES.m_PPU.m_FrameComplete = false;
		}
	}
}

void MainLayer::OnImGuiRender() {
	static bool p_open = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

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

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Options")) {
			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				p_open = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();

	ImGui::Begin("Viewport");
	
	ImVec2 viewpoerPanelSize = ImGui::GetContentRegionAvail();

	uint64_t textureID = m_NES.m_PPU.m_SprScreen->GetRendererID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewpoerPanelSize.x, viewpoerPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();


	ImGui::Begin("Instructions");
	DrawCpu(448, 2);
	ImGui::NewLine();
	DrawCode(448, 72, 26);

	ImGui::Text("C = Step Instruction	F = Step Frame");
	ImGui::Text("R = RESET	Space = Stop / Play");
	ImGui::End();

	ImGui::Begin("RAM");
	// Draw Ram Page 0x00		
	DrawRam(2, 2, 0x0000, 16, 16);
	ImGui::NewLine();
	DrawRam(2, 182, 0x8000, 16, 16);
	ImGui::End();
}

void MainLayer::OnEvent(RealEngine::Event& event) {
	RealEngine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<RealEngine::KeyPressedEvent>(RE_BIND_EVENT_FN(MainLayer::OnKeyPressed));
}

bool MainLayer::OnKeyPressed(RealEngine::KeyPressedEvent& e) {
	if (e.IsRepeating())
		return false;

	if (!m_EmulationRun) {
		// Emulate code step-by-step
		if (e.GetKeyCode() == RealEngine::Key::C) {
			// Clock enough times to execute a whole CPU instruction
			do { m_NES.Clock(); } while (!m_NES.m_6502.Complete());
			// CPU clock runs slower than system clock, so it may be
			// complete for additional system clock cycles. Drain
			// those out
			do { m_NES.Clock(); } while (m_NES.m_6502.Complete());
		}

		// Emulate one whole frame
		if (e.GetKeyCode() == RealEngine::Key::F) {
			// Clock enough times to draw a single frame
			do { m_NES.Clock(); } while (!m_NES.m_PPU.m_FrameComplete);
			// Use residual clock cycles to complete current instruction
			do { m_NES.Clock(); } while (!m_NES.m_6502.Complete());
			// Reset frame completion flag
			m_NES.m_PPU.m_FrameComplete = false;
		}
	}

	if (e.GetKeyCode() == RealEngine::Key::Space) m_EmulationRun = !m_EmulationRun;
	if (e.GetKeyCode() == RealEngine::Key::R) m_NES.Reset();

	return false;
}

void MainLayer::DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
	int nRamX = x, nRamY = y;
	for (int row = 0; row < nRows; row++) {
		std::string sOffset = "$" + hex(nAddr, 4) + ":";
		for (int col = 0; col < nColumns; col++) {
			sOffset += " " + hex(m_NES.CPURead(nAddr, true), 2);
			nAddr += 1;
		}
		ImGui::Text(sOffset.c_str());
		nRamY += 10;
	}
}

void MainLayer::DrawStatus(const char* text, bool enabled) {
	ImGui::PushStyleColor(ImGuiCol_Text, enabled ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
	ImGui::Text(text);
	ImGui::PopStyleColor();
}

void MainLayer::DrawCpu(int x, int y) {
	std::string status = "STATUS: ";
	ImGui::Text("STATUS:");
	DrawStatus("N", m_NES.m_6502.m_Status & Virtual6502::N);
	ImGui::SameLine();
	DrawStatus("V", m_NES.m_6502.m_Status & Virtual6502::V);
	ImGui::SameLine();
	DrawStatus("U", m_NES.m_6502.m_Status & Virtual6502::U);
	ImGui::SameLine();
	DrawStatus("B", m_NES.m_6502.m_Status & Virtual6502::B);
	ImGui::SameLine();
	DrawStatus("D", m_NES.m_6502.m_Status & Virtual6502::D);
	ImGui::SameLine();
	DrawStatus("I", m_NES.m_6502.m_Status & Virtual6502::I);
	ImGui::SameLine();
	DrawStatus("Z", m_NES.m_6502.m_Status & Virtual6502::Z);
	ImGui::SameLine();
	DrawStatus("C", m_NES.m_6502.m_Status & Virtual6502::C);
	ImGui::Text(("PC: $" + hex(m_NES.m_6502.m_Pc, 4)).c_str());
	ImGui::Text(("A: $" + hex(m_NES.m_6502.m_A, 2) + "  [" + std::to_string(m_NES.m_6502.m_A) + "]").c_str());
	ImGui::Text(("X: $" + hex(m_NES.m_6502.m_X, 2) + "  [" + std::to_string(m_NES.m_6502.m_X) + "]").c_str());
	ImGui::Text(("Y: $" + hex(m_NES.m_6502.m_Y, 2) + "  [" + std::to_string(m_NES.m_6502.m_Y) + "]").c_str());
	ImGui::Text(("Stack P: $" + hex(m_NES.m_6502.m_Stkp, 4)).c_str());
}
