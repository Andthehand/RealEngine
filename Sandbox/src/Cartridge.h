#pragma once
#include "Mapper000.h"

#include <RealEngine.h>

#include <filesystem>

struct Flag6_ {
	//Mirroring: 0: horizontal (vertical arrangement) (CIRAM A10 = PPU A11)
	//           1: vertical(horizontal arrangement) (CIRAM A10 = PPU A10)
	unsigned char Mirrored : 1;
	//Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory
	unsigned char Persistence : 1;
	//12-byte trainer at $7000-$71FF (stored before PRG data)
	unsigned char Trainer : 1;
	//Ignore mirroring control or above mirroring bit; instead provide four-screen VRAM
	unsigned char FourScreenVram : 1;
	//Lower nybble of mapper number
	unsigned char LowerMapperData : 4;
};

struct Flag7_ {
	unsigned char VSUnisystem : 1;
	//PlayChoice-10 (8 KB of Hint Screen data stored after CHR data)
	unsigned char PlayChoice : 1;
	//If equal to 2, flags 8-15 are in NES 2.0 format
	unsigned char Format : 2;
	//Upper nybble of mapper number
	unsigned char UpperMapperData : 4;
};

struct Flag8_ {
	unsigned char PRGRAMSize : 8;
};

struct Flag9_ {
	//TV system (0: NTSC; 1: PAL)
	unsigned char TVSystem : 1;
	//Reserved, set to zero
	unsigned char Reserved : 7;
};

struct Flag10_ {
	//TV system (0: NTSC; 2: PAL; 1/3: dual compatible)
	unsigned char TVSystem : 2;
	//PRG RAM ($6000-$7FFF) (0: present; 1: not present)
	unsigned char PRGRAM : 1;
	//Board has no bus conflicts; 1: Board has bus conflicts
	unsigned char BusConflict : 1;
};

struct Header {
	char Name[4];
	
	uint8_t PRGROMSize;
	uint8_t CHRROMSize;

	Flag6_ Flag6;
	Flag7_ Flag7;
	Flag8_ Flag8;
	Flag9_ Flag9;
	Flag10_ Flag10;

	uint32_t Unused;
};

class Cartridge {
public:
	Cartridge(std::filesystem::path& path);

	// Communication with Main Bus
	bool CPURead(uint16_t addr, uint8_t& data);
	bool CPUWrite(uint16_t addr, uint8_t data);

	// Communication with PPU Bus
	bool PPURead(uint16_t addr, uint8_t& data);
	bool PPUWrite(uint16_t addr, uint8_t data);

	bool ImageValid() { return m_ImageValid; }
private:
	bool m_ImageValid = false;
	Header m_Header;

	uint8_t m_MapperID;
	enum MIRROR {
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} m_Mirror = HORIZONTAL;

	uint8_t m_nPRGBanks = 0;
	uint8_t m_nCHRBanks = 0;

	std::vector<uint8_t> m_PRGMemory;
	std::vector<uint8_t> m_CHRMemory;

	RealEngine::Ref<Mapper> m_Mapper;
};
