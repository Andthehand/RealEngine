#include "MainLayer.h"

#include <imgui.h>

MainLayer::MainLayer() : RealEngine::Layer("MainLayer") { }

Bus nes;
std::map<uint16_t, std::string> mapAsm;

std::string hex(uint32_t n, uint8_t d) {
	std::string s(d, '0');
	for (int i = d - 1; i >= 0; i--, n >>= 4)
		s[i] = "0123456789ABCDEF"[n & 0xF];
	return s;
};

void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
	int nRamX = x, nRamY = y;
	for (int row = 0; row < nRows; row++) {
		std::string sOffset = "$" + hex(nAddr, 4) + ":";
		for (int col = 0; col < nColumns; col++) {
			sOffset += " " + hex(nes.Read(nAddr, true), 2);
			nAddr += 1;
		}
		ImGui::Text(sOffset.c_str());
		nRamY += 10;
	}
}

void DrawStatus(const char* text, bool enabled) {
	ImGui::PushStyleColor(ImGuiCol_Text, enabled ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
	ImGui::Text(text);
	ImGui::PopStyleColor();
}

void DrawCpu(int x, int y) {
	std::string status = "STATUS: ";
	ImGui::Text("STATUS:");
	DrawStatus("N", nes.m_6502.m_Status & Virtual6502::N);
	ImGui::SameLine();
	DrawStatus("V", nes.m_6502.m_Status & Virtual6502::V);
	ImGui::SameLine();
	DrawStatus("U", nes.m_6502.m_Status & Virtual6502::U);
	ImGui::SameLine();
	DrawStatus("B", nes.m_6502.m_Status & Virtual6502::B);
	ImGui::SameLine();
	DrawStatus("D", nes.m_6502.m_Status & Virtual6502::D);
	ImGui::SameLine();
	DrawStatus("I", nes.m_6502.m_Status & Virtual6502::I);
	ImGui::SameLine();
	DrawStatus("Z", nes.m_6502.m_Status & Virtual6502::Z);
	ImGui::SameLine();
	DrawStatus("C", nes.m_6502.m_Status & Virtual6502::C);
	ImGui::Text(("PC: $" + hex(nes.m_6502.m_Pc, 4)).c_str());
	ImGui::Text(("A: $" + hex(nes.m_6502.m_A, 2) + "  [" + std::to_string(nes.m_6502.m_A) + "]").c_str());
	ImGui::Text(("X: $" + hex(nes.m_6502.m_X, 2) + "  [" + std::to_string(nes.m_6502.m_X) + "]").c_str());
	ImGui::Text(("Y: $" + hex(nes.m_6502.m_Y, 2) + "  [" + std::to_string(nes.m_6502.m_Y) + "]").c_str());
	ImGui::Text(("Stack P: $" + hex(nes.m_6502.m_Stkp, 4)).c_str());
}

void DrawCode(int x, int y, int nLines) {
	auto it_a = mapAsm.find(nes.m_6502.m_Pc);
	int nLineY = (nLines >> 1) * 10 + y;
	if (it_a != mapAsm.end()) {
		ImGui::Text((*it_a).second.c_str());
		while (nLineY < (nLines * 10) + y) {
			nLineY += 10;
			if (++it_a != mapAsm.end()) {
				ImGui::Text(((*it_a).second).c_str());
			}
		}
	}

	it_a = mapAsm.find(nes.m_6502.m_Pc);
	nLineY = (nLines >> 1) * 10 + y;
	if (it_a != mapAsm.end()) {
		while (nLineY > y) {
			nLineY -= 10;
			if (--it_a != mapAsm.end()) {
				ImGui::Text(((*it_a).second).c_str());
			}
		}
	}
}

void MainLayer::OnAttach() {
	// Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
	/*
		*=$8000
		LDX #10
		STX $0000
		LDX #3
		STX $0001
		LDY $0000
		LDA #0
		CLC
		loop
		ADC $0001
		DEY
		BNE loop
		STA $0002
		NOP
		NOP
		NOP
	*/

	// Convert hex string into bytes for RAM
	std::stringstream ss;
	ss << "A2 03 8E 00 00 AC 00 00 A2 0A 6D 00 00 8D 01 00 CA D0 F7";
	uint16_t nOffset = 0x8000;
	while (!ss.eof()) {
		std::string b;
		ss >> b;
		nes.m_RAM[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
	}

	// Set Reset Vector
	nes.m_RAM[0xFFFC] = 0x00;
	nes.m_RAM[0xFFFD] = 0x80;

	// Dont forget to set IRQ and NMI vectors if you want to play with those

	// Extract dissassembly
	mapAsm = nes.m_6502.Disassemble(0x0000, 0xFFFF);

	// Reset
	nes.m_6502.Reset();
}

void MainLayer::OnUpdate(RealEngine::Timestep ts) {
	RealEngine::RenderCommand::Clear();

	static bool spacePressed = false;
	if (RealEngine::Input::IsKeyPressed(RealEngine::Key::Space) && !spacePressed) {
		spacePressed = true;
		do {
			nes.m_6502.Clock();
		} while (!nes.m_6502.Complete());
	}
	else if (!RealEngine::Input::IsKeyPressed(RealEngine::Key::Space)) {
		spacePressed = false;
	}

	if (RealEngine::Input::IsKeyPressed(RealEngine::Key::R))
		nes.m_6502.Reset();

	if (RealEngine::Input::IsKeyPressed(RealEngine::Key::I))
		nes.m_6502.Irq();

	if (RealEngine::Input::IsKeyPressed(RealEngine::Key::N))
		nes.m_6502.Nmi();

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


	ImGui::Begin("Instructions");
	DrawCpu(448, 2);
	ImGui::NewLine();
	DrawCode(448, 72, 26);

	ImGui::Text("SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI");
	ImGui::End();

	ImGui::Begin("RAM");
	// Draw Ram Page 0x00		
	DrawRam(2, 2, 0x0000, 16, 16);
	ImGui::NewLine();
	DrawRam(2, 182, 0x8000, 16, 16);
	ImGui::End();
}
