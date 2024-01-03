#pragma once
#include <cstdint>

class Mapper {
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper() = default;

public:
	// Transform CPU bus address into PRG ROM offset
	virtual bool CPUMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool CPUMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;
	// Transform PPU bus address into CHR ROM offset
	virtual bool PPUMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool PPUMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

protected:
	// These are stored locally as many of the mappers require this information
	uint8_t m_nPRGBanks = 0;
	uint8_t m_nCHRBanks = 0;
};
