#pragma once
#include "Mapper.h"

class Mapper000 : public Mapper {
public:
	Mapper000(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper000() = default;

	bool CPUMapRead(uint16_t addr, uint32_t& mapped_addr) override;
	bool CPUMapWrite(uint16_t addr, uint32_t& mapped_addr) override;
	
	bool PPUMapRead(uint16_t addr, uint32_t& mapped_addr) override;
	bool PPUMapWrite(uint16_t addr, uint32_t& mapped_addr) override;

	// No local equipment required
};
