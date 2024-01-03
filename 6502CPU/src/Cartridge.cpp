#include "Cartridge.h"
#include <fstream>

#include <RealEngine.h>

Cartridge::Cartridge(std::filesystem::path& path) {
	m_ImageValid = false;

	std::fstream inRom;
	inRom.open(path, std::fstream::in | std::fstream::binary);

	if (inRom.fail()) {
		RE_ERROR("Rom ({0}) failed to load", path.string());
		return;
	}

	uint32_t size = sizeof(Header);
	inRom.read((char*)&m_Header, sizeof(Header));

	if (std::string(m_Header.Name, 4) != "NES\x1A")
		RE_ASSERT(false, "NES does not start the beginning of the rom");

	// If a "trainer" exists we just need to read past
		// it before we get to the good stuff
	if (m_Header.Flag6.Trainer)
		inRom.seekg(512, std::ios_base::cur);

	m_MapperID = (m_Header.Flag7.UpperMapperData << 4) & (m_Header.Flag6.LowerMapperData << 0);
	m_Mirror = (m_Header.Flag6.Mirrored) ? VERTICAL : HORIZONTAL;

	// Load appropriate mapper
	switch (m_MapperID) {
		case 0: m_Mapper = std::make_shared<Mapper000>(m_nPRGBanks, m_nCHRBanks); break;
	}

	m_nPRGBanks = m_Header.PRGROMSize;
	m_PRGMemory.resize(m_nPRGBanks * 16384);
	inRom.read((char*)m_PRGMemory.data(), m_PRGMemory.size());

	m_nCHRBanks = m_Header.CHRROMSize;
	m_CHRMemory.resize(m_nCHRBanks * 8192);
	inRom.read((char*)m_CHRMemory.data(), m_CHRMemory.size());

	m_ImageValid = true;
	inRom.close();
}

bool Cartridge::CPURead(uint16_t addr, uint8_t& data) {
	uint32_t mapped_addr = 0;
	if (m_Mapper->CPUMapRead(addr, mapped_addr)) {
		data = m_PRGMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::CPUWrite(uint16_t addr, uint8_t data) {
	uint32_t mapped_addr = 0;
	if (m_Mapper->CPUMapWrite(addr, mapped_addr)) {
		m_PRGMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::PPURead(uint16_t addr, uint8_t& data) {
	uint32_t mapped_addr = 0;
	if (m_Mapper->PPUMapRead(addr, mapped_addr)) {
		data = m_CHRMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::PPUWrite(uint16_t addr, uint8_t data) {
	uint32_t mapped_addr = 0;
	if (m_Mapper->PPUMapRead(addr, mapped_addr)) {
		m_CHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}
