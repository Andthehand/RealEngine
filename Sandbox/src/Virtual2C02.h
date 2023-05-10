#pragma once
#include "Cartridge.h"

#include <RealEngine.h>

using namespace RealEngine;

struct Color {
	union {
		uint32_t n = 0xFFFFFFFF;
		struct { uint8_t R; uint8_t G; uint8_t B; uint8_t A; };
	};

	Color() = default;
	Color(uint32_t p) { n = p; }
	Color(uint8_t r, uint8_t g, uint8_t b) { R = r; G = g; B = b; A = 255; }
};

class Virtual2C02 {
public:
	Virtual2C02();
	~Virtual2C02();

	// Communications with Main Bus
	uint8_t CPURead(uint16_t addr, bool rdonly = false);
	void    CPUWrite(uint16_t addr, uint8_t  data);

	// Communications with PPU Bus
	uint8_t PPURead(uint16_t addr, bool rdonly = false);
	void    PPUWrite(uint16_t addr, uint8_t data);

	void Clock();

	// Interface
	void ConnectCartridge(const RealEngine::Ref<Cartridge> cartridge) { m_Cartridge = cartridge; }
public:
	Ref<Texture2D> m_SprScreen;
	Ref<Texture2D> m_SprNameTable[2];
	Ref<Texture2D> m_SprPatternTable[2];

	bool m_FrameComplete = false;
private:
	Color* m_Pixels;
	Color* m_PalScreen;

	// The Cartridge or "GamePak"
	RealEngine::Ref<Cartridge> m_Cartridge;

	int16_t m_Scanline = 0;
	int16_t m_Cycle = 0;
};
