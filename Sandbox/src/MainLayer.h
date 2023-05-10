#pragma once
#include <RealEngine.h>

#include "Cartridge.h"
#include "Bus.h"

class MainLayer : public RealEngine::Layer {
public:
	MainLayer();
	virtual void OnAttach() override;
	virtual void OnUpdate(RealEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(RealEngine::Event& event) override;

private:
	bool OnKeyPressed(RealEngine::KeyPressedEvent& e);

	void DrawCpu(int x, int y);
	void DrawCode(int x, int y, int nLines);
	void DrawStatus(const char* text, bool enabled);
	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);

private:
	Bus m_NES;
	std::map<uint16_t, std::string> m_MapAsm;

	RealEngine::Ref<Cartridge> m_Cartridge;

	bool m_EmulationRun = false;
	float m_ResidualTime = 0.0f;
};
