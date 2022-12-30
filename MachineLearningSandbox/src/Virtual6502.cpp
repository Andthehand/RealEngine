#include "Virtual6502.h"
#include "Bus.h"

Virtual6502::Virtual6502() {
	using a = Virtual6502;
	m_Lookup = {
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::Bit, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::Bit, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

Virtual6502::~Virtual6502() {

}

std::map<uint16_t, std::string> Virtual6502::Disassemble(uint16_t nStart, uint16_t nStop) {
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d) {
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop) {
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = m_Bus->Read(addr, true); addr++;
		sInst += m_Lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (m_Lookup[opcode].Addrmode == &Virtual6502::IMP) {
			sInst += " {IMP}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::IMM) {
			value = m_Bus->Read(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::ZP0) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::ZPX) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::ZPY) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::IZX) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::IZY) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::ABS) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = m_Bus->Read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::ABX) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = m_Bus->Read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::ABY) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = m_Bus->Read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::IND) {
			lo = m_Bus->Read(addr, true); addr++;
			hi = m_Bus->Read(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (m_Lookup[opcode].Addrmode == &Virtual6502::REL) {
			value = m_Bus->Read(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + ((uint16_t)value | 0xFF00), 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}

uint8_t Virtual6502::Read(uint16_t addr) {
	return m_Bus->Read(addr);
}

void Virtual6502::Write(uint16_t addr, uint8_t data) {
	m_Bus->Write(addr, data);
}

void Virtual6502::Clock() {
	if (m_Cycles == 0) {
		m_Opcode = Read(m_Pc);
		m_Pc++;

		m_Cycles = m_Lookup[m_Opcode].cycles;

		uint8_t additionalCycle1 = (this->*m_Lookup[m_Opcode].Addrmode)();
	
		uint8_t additionalCycle2 = (this->*m_Lookup[m_Opcode].Operate)();

		m_Cycles += (additionalCycle1 & additionalCycle2);
	}

	m_Cycles--;
}

void Virtual6502::Reset() {
	m_A = 0;
	m_X = 0;
	m_Y = 0;
	m_Stkp = 0xFD;
	m_Status = 0x00 | U;

	m_AddrAbs = 0xFFFC;
	uint16_t lo = Read(m_AddrAbs + 0);
	uint16_t hi = Read(m_AddrAbs + 1);

	m_Pc = (hi << 8) | lo;

	m_AddrAbs = 0x0000;
	m_AddrRel = 0x0000;
	m_Fetched = 0x00;

	m_Cycles = 8;
}

//Interupt request
void Virtual6502::Irq() {
	// If interrupts are allowed
	if (GetFlag(I) == 0) {
		// Push the program counter to the stack. It's 16-bits dont
		// forget so that takes two pushes
		Write(0x0100 + m_Stkp, (m_Pc >> 8) & 0x00FF);
		m_Stkp--;
		Write(0x0100 + m_Stkp, m_Pc & 0x00FF);
		m_Stkp--;

		// Then Push the status register to the stack
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		Write(0x0100 + m_Stkp, m_Status);
		m_Stkp--;

		// Read new program counter location from fixed address
		m_AddrAbs = 0xFFFE;
		uint16_t lo = Read(m_AddrAbs + 0);
		uint16_t hi = Read(m_AddrAbs + 1);
		m_Pc = (hi << 8) | lo;

		// IRQs take time
		m_Cycles = 7;
	}
}


//Non Maskable Interupt request
void Virtual6502::Nmi() {
	//Store the current Program counter so that you can go back to it
	Write(0x0100 + m_Stkp, (m_Pc >> 8) & 0x00FF);
	m_Stkp--;
	Write(0x0100 + m_Stkp, m_Pc & 0x00FF);
	m_Stkp--;

	//Reset Values
	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	Write(0x0100 + m_Stkp, m_Status);
	m_Stkp--;

	//Check what to do when interupted
	m_AddrAbs = 0xFFFA;
	uint16_t lo = Read(m_AddrAbs + 0);
	uint16_t hi = Read(m_AddrAbs + 1);
	m_Pc = (hi << 8) | lo;

	m_Cycles = 8;
}

uint8_t Virtual6502::GetFlag(FLAGS6502 f) {
	return ((m_Status & f) > 0) ? 1 : 0;
}

void Virtual6502::SetFlag(FLAGS6502 f, bool v) {
	if (v)
		m_Status |= f;
	else
		m_Status &= ~f;
}

//Adress Modes
//Implied
uint8_t Virtual6502::IMP() {
	m_Fetched = m_A;
	return 0;
}

//Immediate
uint8_t Virtual6502::IMM() {
	m_AddrAbs = m_Pc++;
	return 0;
}

//Zero Page
uint8_t Virtual6502::ZP0() {
	m_AddrAbs = Read(m_Pc);
	m_Pc++;
	m_AddrAbs &= 0x00FF;
	return 0;
}

//Zero Page with X Offset
uint8_t Virtual6502::ZPX() {
	m_AddrAbs = (Read(m_Pc) + m_X);
	m_Pc++;
	m_AddrAbs &= 0x00FF;
	return 0;
}

//Zero Page with Y Offset
uint8_t Virtual6502::ZPY() {
	m_AddrAbs = (Read(m_Pc) + m_Y);
	m_Pc++;
	m_AddrAbs &= 0x00FF;
	return 0;
}

//Relative
uint8_t Virtual6502::REL() {
	m_AddrRel = Read(m_Pc);
	m_Pc++;
	if (m_AddrRel & 0x80)
		m_AddrRel |= 0xFF00;

	return 0;
}

//Absolute
uint8_t Virtual6502::ABS() {
	uint16_t lo = Read(m_Pc);
	m_Pc++;
	uint16_t hi = Read(m_Pc);
	m_Pc++;

	m_AddrAbs = (hi << 8) | lo;
	return 0;
}

//Absolute with X Offset
uint8_t Virtual6502::ABX() {
	uint16_t lo = Read(m_Pc);
	m_Pc++;
	uint16_t hi = Read(m_Pc);
	m_Pc++;

	m_AddrAbs = (hi << 8) | lo;
	m_AddrAbs += m_X;

	if ((m_AddrAbs & 0xFF00) != (hi < 8))
		return 1;
	else
		return 0;
}

//Absolute with Y Offset
uint8_t Virtual6502::ABY() {
	uint16_t lo = Read(m_Pc);
	m_Pc++;
	uint16_t hi = Read(m_Pc);
	m_Pc++;

	m_AddrAbs = (hi << 8) | lo;
	m_AddrAbs += m_Y;

	if ((m_AddrAbs & 0xFF00) != (hi < 8))
		return 1;
	else
		return 0;
}

//Indirect
uint8_t Virtual6502::IND() {
	uint16_t ptrLo = Read(m_Pc);
	m_Pc++;
	uint16_t ptrHi = Read(m_Pc);
	m_Pc++;

	uint16_t ptr = (ptrHi << 8) | ptrLo;
	
	if (ptrLo == 0x00FF) {
		m_AddrAbs = (Read(ptr & 0xFF00) << 8) | Read(ptr + 0);

	}
	else {
		m_AddrAbs = (Read(ptr + 1) << 8) | Read(ptr + 0);
	}

	return 0;
}

//Indirect X
uint8_t Virtual6502::IZX() {
	uint16_t t = Read(m_Pc);
	m_Pc++;

	uint16_t lo = Read((uint16_t)(t + (uint16_t)m_X) & 0x00FF);
	uint16_t hi = Read((uint16_t)(t + (uint16_t)m_X + 1) & 0x00FF);

	m_AddrAbs = (hi << 8) | lo;

	return 0;
}

//Indirect Y
uint8_t Virtual6502::IZY() {
	uint16_t t = Read(m_Pc);
	m_Pc++;

	uint16_t lo = Read((t + (uint16_t)m_Y)) & 0x00FF;
	uint16_t hi = Read((uint16_t)(t + (uint16_t)m_Y + 1) & 0x00FF);

	m_AddrAbs = (hi << 8) | lo;
	m_AddrAbs += m_Y;
	if ((m_AddrAbs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

//Instructions

uint8_t Virtual6502::Fetch() {
	if (!(m_Lookup[m_Opcode].Addrmode == &Virtual6502::IMP))
		m_Fetched = Read(m_AddrAbs);
	return m_Fetched;
}

//Add with carry
uint8_t Virtual6502::ADC() {
	Fetch();

	//Do the actuall addition here ofc with carry
	uint16_t temp = (uint16_t)m_A + (uint16_t)m_Fetched + (uint16_t)GetFlag(C);
	SetFlag(C, temp > 255);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x80);

	//Because of complicated negative number wrap around crap
	SetFlag(V, ((~(uint16_t)m_A ^ (uint16_t)m_Fetched) & ((uint16_t)m_A ^ (uint16_t)temp)) & 0x0080);

	m_A = temp & 0x00FF;

	return 1;
}

uint8_t Virtual6502::AND() {
	Fetch();
	m_A = m_A & m_Fetched;
	//If Zero
	SetFlag(Z, m_A == 0x00);
	//If Negative
	SetFlag(N, m_A & 0x80);
	return 1;
}

//Arithmetic Shift Left
//Function: A = C <- (A << 1) <- 0
uint8_t Virtual6502::ASL() {
	Fetch();

	uint16_t temp = (uint16_t)m_Fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x80);

	if (m_Lookup[m_Opcode].Addrmode == &Virtual6502::IMP)
		m_A = temp & 0x00FF;
	else
		Write(m_AddrAbs, temp & 0x00FF);
	return 0;
}

uint8_t Virtual6502::BCS() {
	if (GetFlag(C) == 1) {
		m_Cycles++;
		m_AddrAbs = m_Pc + m_AddrRel;

		//If a new page add a clock cycle
		if ((m_AddrAbs & 0xFF00) != (m_Pc & 0xFF00))
			m_Cycles++;

		m_Pc = m_AddrAbs;
	}
	return 0;
}

uint8_t Virtual6502::BCC() {
	if (GetFlag(C) == 0) {
		m_Cycles++;
		m_AddrAbs = m_Pc + m_AddrRel;

		//If a new page add a clock cycle
		if ((m_AddrAbs & 0xFF00) != (m_Pc & 0xFF00))
			m_Cycles++;

		m_Pc = m_AddrAbs;
	}
	return 0;
}

uint8_t Virtual6502::BEQ() {
	if (GetFlag(Z) == 1) {
		m_Cycles++;
		m_AddrAbs = m_Pc + m_AddrRel;

		//If a new page add a clock cycle
		if ((m_AddrAbs & 0xFF00) != (m_Pc & 0xFF00))
			m_Cycles++;

		m_Pc = m_AddrAbs;
	}
	return 0;
}

//BIT (test BITs)
uint8_t Virtual6502::Bit() {
	Fetch();
	uint16_t temp = m_Fetched & m_A;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(V, m_Fetched & (1 << 6));
	SetFlag(N, m_Fetched & (1 << 7));

	return 0;
}

//Branch Instructions
//Branch on Plus
uint8_t Virtual6502::BPL() {
	if (GetFlag(N) == 0) {
		m_Cycles++;
		m_AddrAbs = m_Pc + m_AddrRel;

		//If a new page add a clock cycle
		if ((m_AddrAbs & 0xFF00) != (m_Pc & 0xFF00))
			m_Cycles++;

		m_Pc = m_AddrAbs;
	}
	return 0;
}

//Branch on Minus
uint8_t Virtual6502::BMI() {
	if (GetFlag(N) == 1) {
		m_Cycles++;
		m_AddrAbs = m_Pc + m_AddrRel;

		//If a new page add a clock cycle
		if ((m_AddrAbs & 0xFF00) != (m_Pc & 0xFF00))
			m_Cycles++;

		m_Pc = m_AddrAbs;
	}
	return 0;
}

//Branch on Overflow Clear
uint8_t Virtual6502::BVC() {
	if (GetFlag(V) == 0) {
		m_Cycles++;
		m_AddrAbs = m_Pc + m_AddrRel;

		//If a new page add a clock cycle
		if ((m_AddrAbs & 0xFF00) != (m_Pc & 0xFF00))
			m_Cycles++;

		m_Pc = m_AddrAbs;
	}
	return 0;
}

//Branch on Overflow Set
uint8_t Virtual6502::BVS() {
	if (GetFlag(V) == 1) {
		m_Cycles++;
		m_AddrAbs = m_Pc + m_AddrRel;

		//If a new page add a clock cycle
		if ((m_AddrAbs & 0xFF00) != (m_Pc & 0xFF00))
			m_Cycles++;

		m_Pc = m_AddrAbs;
	}
	return 0;
}

//Branch on Zero
uint8_t Virtual6502::BNE() {
	if (GetFlag(Z) == 0) {
		m_Cycles++;
		m_AddrAbs = m_Pc + m_AddrRel;

		//If a new page add a clock cycle
		if ((m_AddrAbs & 0xFF00) != (m_Pc & 0xFF00))
			m_Cycles++;

		m_Pc = m_AddrAbs;
	}
	return 0;
}

//Break
uint8_t Virtual6502::BRK() {
	m_Pc++;

	//Trigger a Non Maskable Interupt
	SetFlag(I, 1);
	Write(0x0100 + m_Stkp, (m_Pc >> 8) & 0x00FF);
	m_Stkp--;
	Write(0x0100 + m_Stkp, m_Pc & 0x00FF);
	m_Stkp--;

	//Set break
	SetFlag(B, 1);
	Write(0x0100 + m_Stkp, m_Status);
	m_Stkp--;
	SetFlag(B, 0);

	m_Pc = (uint16_t)Read(0xFFFE) | ((uint16_t)Read(0xFFFF) << 8);
	return 0;
}


//Clear Carry flag
uint8_t Virtual6502::CLC() {
	SetFlag(C, false);
	return 0;
}

//Clear Decimal Mode flag
uint8_t Virtual6502::CLD() {
	SetFlag(D, false);
	return 0;
}

//Clear Disable Interrupts flag
uint8_t Virtual6502::CLI() {
	SetFlag(I, false);
	return 0;
}

//Clear Overload flag
uint8_t Virtual6502::CLV() {
	SetFlag(V, false);
	return 0;
}

//Compare accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
uint8_t Virtual6502::CMP() {
	Fetch();

	uint16_t temp = (uint16_t)m_A - (uint16_t)m_Fetched;
	SetFlag(C, m_A >= m_Fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}

//Compare X register
uint8_t Virtual6502::CPX() {
	Fetch();

	uint16_t temp = (uint16_t)m_X - (uint16_t)m_Fetched;
	SetFlag(C, m_X >= m_Fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

//Compare Y register
uint8_t Virtual6502::CPY() {
	Fetch();

	uint16_t temp = (uint16_t)m_Y - (uint16_t)m_Fetched;
	SetFlag(C, m_Y >= m_Fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

//Decrement Value at Memory Location
uint8_t Virtual6502::DEC() {
	Fetch();

	uint16_t temp = m_Fetched - 1;
	Write(m_AddrAbs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	
	return 0;
}

//Decrement X Register
uint8_t Virtual6502::DEX() {
	m_X--;
	SetFlag(Z, m_X == 0x00);
	SetFlag(N, m_X & 0x80);
	return 0;
}

//Decrement Y Register
uint8_t Virtual6502::DEY() {
	m_Y--;
	SetFlag(Z, m_Y == 0x00);
	SetFlag(N, m_Y & 0x80);
	return 0;
}

//Bitwise Logic XOR
uint8_t Virtual6502::EOR() {
	Fetch();
	m_A = m_A ^ m_Fetched;
	SetFlag(Z, m_A == 0x00);
	SetFlag(N, m_A & 0x80);
	return 1;
}

//Increment Value at Memory Location
uint8_t Virtual6502::INC() {
	Fetch();

	uint16_t temp = m_Fetched + 1;
	Write(m_AddrAbs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	return 0;
}

//Increment X
uint8_t Virtual6502::INX() {
	m_X++;
	SetFlag(Z, m_X == 0x00);
	SetFlag(N, m_X & 0x80);
	return 0;
}

//Increment Y
uint8_t Virtual6502::INY() {
	m_Y++;
	SetFlag(Z, m_Y == 0x00);
	SetFlag(N, m_Y & 0x80);
	return 0;
}

//Jump to memory address
uint8_t Virtual6502::JMP() {
	m_Pc = m_AddrAbs;
	return 0;
}

//Jump To Sub-Routine
uint8_t Virtual6502::JSR()
{
	m_Pc--;

	Write(0x0100 + m_Stkp, (m_Pc >> 8) & 0x00FF);
	m_Stkp--;
	Write(0x0100 + m_Stkp, m_Pc & 0x00FF);
	m_Stkp--;

	m_Pc = m_AddrAbs;
	return 0;
}


//Load The Accumulator
uint8_t Virtual6502::LDA()
{
	Fetch();
	m_A = m_Fetched;
	SetFlag(Z, m_A == 0x00);
	SetFlag(N, m_A & 0x80);
	return 1;
}


//Load The X Register
uint8_t Virtual6502::LDX()
{
	Fetch();
	m_X = m_Fetched;
	SetFlag(Z, m_X == 0x00);
	SetFlag(N, m_X & 0x80);
	return 1;
}


//Load The Y Register
uint8_t Virtual6502::LDY()
{
	Fetch();
	m_Y = m_Fetched;
	SetFlag(Z, m_Y == 0x00);
	SetFlag(N, m_Y & 0x80);
	return 1;
}

uint8_t Virtual6502::LSR() {
	Fetch();
	SetFlag(C, m_Fetched & 0x0001);
	uint16_t temp = m_Fetched >> 1;
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (m_Lookup[m_Opcode].Addrmode == &Virtual6502::IMP)
		m_A = temp & 0x00FF;
	else
		Write(m_AddrAbs, temp & 0x00FF);
	return 0;
}

uint8_t Virtual6502::NOP()
{
	switch (m_Opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


//Bitwise Logic OR
uint8_t Virtual6502::ORA() {
	Fetch();
	m_A = m_A | m_Fetched;
	SetFlag(Z, m_A == 0x00);
	SetFlag(N, m_A & 0x80);
	return 1;
}

//Push Status Register to Stack
uint8_t Virtual6502::PHP() {
	Write(0x0100 + m_Stkp, m_Status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	m_Stkp--;
	return 0;
}

//Push Accumulator to the stack
uint8_t Virtual6502::PHA() {
	Write(0x0100 + m_Stkp, m_A);
	m_Stkp--;
	return 0;
}

//Pop stack
uint8_t Virtual6502::PLA() {
	m_Stkp++;
	m_A = Read(0x0100 + m_Stkp);
	SetFlag(Z, m_A == 0x00);
	SetFlag(Z, m_A == 0x80);
	return 0;
}

//Pop Status Register off Stack
uint8_t Virtual6502::PLP() {
	m_Stkp++;
	m_Status = Read(0x0100 + m_Stkp);
	SetFlag(U, 1);
	return 0;
}

uint8_t Virtual6502::ROL() {
	Fetch();
	uint16_t temp = (uint16_t)(m_Fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (m_Lookup[m_Opcode].Addrmode == &Virtual6502::IMP)
		m_A = temp & 0x00FF;
	else
		Write(m_AddrAbs, temp & 0x00FF);
	return 0;
}

uint8_t Virtual6502::ROR() {
	Fetch();
	uint16_t temp = (uint16_t)(GetFlag(C) << 7) | (m_Fetched >> 1);
	SetFlag(C, m_Fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (m_Lookup[m_Opcode].Addrmode == &Virtual6502::IMP)
		m_A = temp & 0x00FF;
	else
		Write(m_AddrAbs, temp & 0x00FF);
	return 0;
}

//Return from interupt
uint8_t Virtual6502::RTI() {
	m_Stkp++;
	m_Status = Read(0x0100 + m_Stkp);
	m_Status &= ~B;
	m_Status &= ~U;

	m_Stkp++;
	m_Pc = (uint16_t)Read(0x0100 + m_Stkp);
	m_Stkp++;
	m_Pc |= (uint16_t)Read(0x0100 + m_Stkp) << 8;
	return 0;
}

uint8_t Virtual6502::RTS() {
	m_Stkp++;
	m_Pc = (uint16_t)Read(0x0100 + m_Stkp);
	m_Stkp++;
	m_Pc |= (uint16_t)Read(0x0100 + m_Stkp) << 8;

	m_Pc++;
	return 0;
}

//Subtract with Carry
uint8_t Virtual6502::SBC() {
	Fetch();

	// We can invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)m_Fetched) ^ 0x00FF;

	uint16_t temp = (uint16_t)m_A + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(V, (temp ^ (uint16_t)m_A) & (temp ^ value) & 0x0080);
	SetFlag(N, temp & 0x0080);
	m_A = temp & 0x00FF;
	return 1;
}

//Set Carry Flag
// Function:    C = 1
uint8_t Virtual6502::SEC() {
	SetFlag(C, true);
	return 0;
}


//Set Decimal Flag
uint8_t Virtual6502::SED() {
	SetFlag(D, true);
	return 0;
}


//Set Interrupt Flag / Enable Interrupts
uint8_t Virtual6502::SEI() {
	SetFlag(I, true);
	return 0;
}


//Store Accumulator at Address
uint8_t Virtual6502::STA() {
	Write(m_AddrAbs, m_A);
	return 0;
}


//Store X Register at Address
uint8_t Virtual6502::STX() {
	Write(m_AddrAbs, m_X);
	return 0;
}


//Store Y Register at Address
uint8_t Virtual6502::STY() {
	Write(m_AddrAbs, m_Y);
	return 0;
}


//Transfer Accumulator to X Register
uint8_t Virtual6502::TAX() {
	m_X = m_A;
	SetFlag(Z, m_X == 0x00);
	SetFlag(N, m_X & 0x80);
	return 0;
}


//Transfer Accumulator to Y Register
uint8_t Virtual6502::TAY() {
	m_Y = m_A;
	SetFlag(Z, m_Y == 0x00);
	SetFlag(N, m_Y & 0x80);
	return 0;
}


//Transfer Stack Pointer to X Register
uint8_t Virtual6502::TSX() {
	m_X = m_Stkp;
	SetFlag(Z, m_X == 0x00);
	SetFlag(N, m_X & 0x80);
	return 0;
}


//Transfer X Register to Accumulator
uint8_t Virtual6502::TXA() {
	m_A = m_X;
	SetFlag(Z, m_A == 0x00);
	SetFlag(N, m_A & 0x80);
	return 0;
}


//Transfer X Register to Stack Pointer
uint8_t Virtual6502::TXS() {
	m_Stkp = m_X;
	return 0;
}


//Transfer Y Register to Accumulator
uint8_t Virtual6502::TYA() {
	m_A = m_Y;
	SetFlag(Z, m_A == 0x00);
	SetFlag(N, m_A & 0x80);
	return 0;
}


//This function captures illegal opcodes
uint8_t Virtual6502::XXX() {
	return 0;
}
