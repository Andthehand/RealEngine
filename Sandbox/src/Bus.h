#pragma once
#include "Virtual6502.h"
#include "Virtual2C02.h"
#include "Cartridge.h"

#include <RealEngine.h>

#include <cstdint>
#include <array>

class Bus {
public:
	Bus();
	~Bus();

	//Read and Writes
	void CPUWrite(uint16_t addr, uint8_t data);
	uint8_t CPURead(uint16_t addr, bool bReadOnly = false);

	// Connects a cartridge object to the internal buses
	void InsertCartridge(const RealEngine::Ref<Cartridge> cartridge);

	// Resets the system
	void Reset();
	void Clock();
public:
	// A count of how many clocks have passed
	uint32_t nSystemClockCounter = 0;

	//Connected to the Bus
	Virtual6502 m_6502;

	Virtual2C02 m_PPU;

	RealEngine::Ref<Cartridge> m_Cartridge;

	std::array<uint8_t, 64 * 1024> m_RAM;
};
