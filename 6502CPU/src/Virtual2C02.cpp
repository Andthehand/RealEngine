#include "Virtual2C02.h"

static const uint32_t WIDTH = 256;
static const uint32_t HEIGHT = 240;

Virtual2C02::Virtual2C02() {
	m_SprScreen = Texture2D::Create(WIDTH, HEIGHT);
	m_Pixels = new Color[WIDTH * HEIGHT];
	m_PalScreen = new Color[0x40];
	m_SprScreen->SetData(m_Pixels, sizeof(Color) * WIDTH * HEIGHT);

	m_PalScreen[0x00] = Color(84, 84, 84);
	m_PalScreen[0x01] = Color(0, 30, 116);
	m_PalScreen[0x02] = Color(8, 16, 144);
	m_PalScreen[0x03] = Color(48, 0, 136);
	m_PalScreen[0x04] = Color(68, 0, 100);
	m_PalScreen[0x05] = Color(92, 0, 48);
	m_PalScreen[0x06] = Color(84, 4, 0);
	m_PalScreen[0x07] = Color(60, 24, 0);
	m_PalScreen[0x08] = Color(32, 42, 0);
	m_PalScreen[0x09] = Color(8, 58, 0);
	m_PalScreen[0x0A] = Color(0, 64, 0);
	m_PalScreen[0x0B] = Color(0, 60, 0);
	m_PalScreen[0x0C] = Color(0, 50, 60);
	m_PalScreen[0x0D] = Color(0, 0, 0);
	m_PalScreen[0x0E] = Color(0, 0, 0);
	m_PalScreen[0x0F] = Color(0, 0, 0);

	m_PalScreen[0x10] = Color(152, 150, 152);
	m_PalScreen[0x11] = Color(8, 76, 196);
	m_PalScreen[0x12] = Color(48, 50, 236);
	m_PalScreen[0x13] = Color(92, 30, 228);
	m_PalScreen[0x14] = Color(136, 20, 176);
	m_PalScreen[0x15] = Color(160, 20, 100);
	m_PalScreen[0x16] = Color(152, 34, 32);
	m_PalScreen[0x17] = Color(120, 60, 0);
	m_PalScreen[0x18] = Color(84, 90, 0);
	m_PalScreen[0x19] = Color(40, 114, 0);
	m_PalScreen[0x1A] = Color(8, 124, 0);
	m_PalScreen[0x1B] = Color(0, 118, 40);
	m_PalScreen[0x1C] = Color(0, 102, 120);
	m_PalScreen[0x1D] = Color(0, 0, 0);
	m_PalScreen[0x1E] = Color(0, 0, 0);
	m_PalScreen[0x1F] = Color(0, 0, 0);

	m_PalScreen[0x20] = Color(236, 238, 236);
	m_PalScreen[0x21] = Color(76, 154, 236);
	m_PalScreen[0x22] = Color(120, 124, 236);
	m_PalScreen[0x23] = Color(176, 98, 236);
	m_PalScreen[0x24] = Color(228, 84, 236);
	m_PalScreen[0x25] = Color(236, 88, 180);
	m_PalScreen[0x26] = Color(236, 106, 100);
	m_PalScreen[0x27] = Color(212, 136, 32);
	m_PalScreen[0x28] = Color(160, 170, 0);
	m_PalScreen[0x29] = Color(116, 196, 0);
	m_PalScreen[0x2A] = Color(76, 208, 32);
	m_PalScreen[0x2B] = Color(56, 204, 108);
	m_PalScreen[0x2C] = Color(56, 180, 204);
	m_PalScreen[0x2D] = Color(60, 60, 60);
	m_PalScreen[0x2E] = Color(0, 0, 0);
	m_PalScreen[0x2F] = Color(0, 0, 0);

	m_PalScreen[0x30] = Color(236, 238, 236);
	m_PalScreen[0x31] = Color(168, 204, 236);
	m_PalScreen[0x32] = Color(188, 188, 236);
	m_PalScreen[0x33] = Color(212, 178, 236);
	m_PalScreen[0x34] = Color(236, 174, 236);
	m_PalScreen[0x35] = Color(236, 174, 212);
	m_PalScreen[0x36] = Color(236, 180, 176);
	m_PalScreen[0x37] = Color(228, 196, 144);
	m_PalScreen[0x38] = Color(204, 210, 120);
	m_PalScreen[0x39] = Color(180, 222, 120);
	m_PalScreen[0x3A] = Color(168, 226, 144);
	m_PalScreen[0x3B] = Color(152, 226, 180);
	m_PalScreen[0x3C] = Color(160, 214, 228);
	m_PalScreen[0x3D] = Color(160, 162, 160);
	m_PalScreen[0x3E] = Color(0, 0, 0);
	m_PalScreen[0x3F] = Color(0, 0, 0);
}

Virtual2C02::~Virtual2C02() {
	delete[] m_Pixels;
	delete[] m_PalScreen;
}

uint8_t Virtual2C02::CPURead(uint16_t addr, bool rdonly) {
	uint8_t data = 0x00;

	switch (addr) {
		case 0x0000: // Control
			break;
		case 0x0001: // Mask
			break;
		case 0x0002: // Status
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
			break;
	}

	return data;
}

void Virtual2C02::CPUWrite(uint16_t addr, uint8_t data) {
	switch (addr) {
		case 0x0000: // Control
			break;
		case 0x0001: // Mask
			break;
		case 0x0002: // Status
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
			break;
	}
}

uint8_t Virtual2C02::PPURead(uint16_t addr, bool rdonly) {
	uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (m_Cartridge->PPURead(addr, data)) {

	}

	return data;
}

void Virtual2C02::PPUWrite(uint16_t addr, uint8_t data) {
	addr &= 0x3FFF;

	if (m_Cartridge->PPUWrite(addr, data)) {

	}
}

void SetPixel(Color* color, int32_t x, int32_t y, Color p) {
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
		color[y * WIDTH + x] = p;
	}
}

void Virtual2C02::Clock() {
	// Fake some noise for now
	SetPixel(m_Pixels, m_Cycle - 1, m_Scanline, m_PalScreen[(rand() % 2) ? 0x3F : 0x30]);

	// Advance renderer - it never stops, it's relentless
	m_Cycle++;
	if (m_Cycle >= 341) {
		m_Cycle = 0;
		m_Scanline++;
		if (m_Scanline >= 261) {
			m_Scanline = -1;
			m_SprScreen->SetData(m_Pixels, sizeof(Color) * WIDTH * HEIGHT);
			m_FrameComplete = true;
		}
	}
}
