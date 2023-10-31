#include "Mapper000.h"

Mapper000::Mapper000(uint8_t prgBanks, uint8_t chrBanks) 
	: Mapper(prgBanks, chrBanks) { }

bool Mapper000::CPUMapRead(uint16_t addr, uint32_t& mapped_addr) {
	// if PRGROM is 16KB
	//     CPU Address Bus          PRG ROM
	//     0x8000 -> 0xBFFF: Map    0x0000 -> 0x3FFF
	//     0xC000 -> 0xFFFF: Mirror 0x0000 -> 0x3FFF
	// if PRGROM is 32KB
	//     CPU Address Bus          PRG ROM
	//     0x8000 -> 0xFFFF: Map    0x0000 -> 0x7FFF	
	if (addr >= 0x8000 && addr <= 0xFFFF) {
		mapped_addr = addr & (m_nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool Mapper000::CPUMapWrite(uint16_t addr, uint32_t& mapped_addr) {
	if (addr >= 0x8000 && addr <= 0xFFFF) {
		mapped_addr = addr & (m_nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool Mapper000::PPUMapRead(uint16_t addr, uint32_t& mapped_addr) {
	// There is no mapping required for PPU
	// PPU Address Bus          CHR ROM
	// 0x0000 -> 0x1FFF: Map    0x0000 -> 0x1FFF
	if (addr >= 0x0000 && addr <= 0x1FFF) {
		mapped_addr = addr;
		return true;
	}

	return false;
}

bool Mapper000::PPUMapWrite(uint16_t addr, uint32_t& mapped_addr) {
	if (addr >= 0x0000 && addr <= 0x1FFF) {
		if (m_nCHRBanks == 0) {
			// Treat as RAM
			mapped_addr = addr;
			return true;
		}
	}

	return false;
}
