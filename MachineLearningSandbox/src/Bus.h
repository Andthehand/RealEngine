#pragma once
#include <cstdint>
#include <array>

#include "Virtual6502.h"

class Bus {
public:
	Bus();
	~Bus();

	//Read and Writes
	void Write(uint16_t addr, uint8_t data);
	uint8_t Read(uint16_t addr, bool bReadOnly = false);


	//Connected to the Bus
	Virtual6502 m_6502;

	std::array<uint8_t, 64 * 1024> m_RAM;
};
