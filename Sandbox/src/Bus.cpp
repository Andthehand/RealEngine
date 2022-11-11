#include "Bus.h"

Bus::Bus() {
	//Clear the memory of the ram
	for (auto& x : m_RAM) x = 0x00;

	//Connect the 6502 so that it doesn't have a nullptr
	m_6502.ConnectBus(this);
}

Bus::~Bus() {

}

void Bus::Write(uint16_t addr, uint8_t data) {
	//Make sure the memory is inside the ram address
	if(addr >= 0x0000 && addr <= 0xFFFF)
		m_RAM[addr] = data;
}

uint8_t Bus::Read(uint16_t addr, bool bReadOnly) {
	//Make sure the memory is inside the ram address
	if (addr >= 0x0000 && addr <= 0xFFFF)
		return m_RAM[addr];

	//The memory is outside all of the addresses
	return 0x00;
}
