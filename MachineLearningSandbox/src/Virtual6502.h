#pragma once
#include <string>
#include <vector>
#include <map>

class Bus;

class Virtual6502 {
public:
	Virtual6502();
	~Virtual6502();

	enum FLAGS6502 {
		C = (1 << 0), //Carry Bit
		Z = (1 << 1), //Zero
		I = (1 << 2), //Disable Interrupts
		D = (1 << 3), //Deciaml Mode
		B = (1 << 4), //Break
		U = (1 << 5), //Unused
		V = (1 << 6), //Overflow
		N = (1 << 7), //Negative
	};

	bool Complete() { return m_Cycles == 0; }

	void ConnectBus(Bus* n) { m_Bus = n; }

	std::map<uint16_t, std::string> Disassemble(uint16_t nStart, uint16_t nStop);

	//Addressing Modes
	uint8_t IMP(); uint8_t IMM();
	uint8_t ZP0(); uint8_t ZPX();
	uint8_t ZPY(); uint8_t REL();
	uint8_t ABS(); uint8_t ABX();
	uint8_t ABY(); uint8_t IND();
	uint8_t IZX(); uint8_t IZY();

	//Opcodes
	uint8_t ADC(); uint8_t AND(); uint8_t ASL(); uint8_t BCC();
	uint8_t BCS(); uint8_t BEQ(); uint8_t Bit(); uint8_t BMI();
	uint8_t BNE(); uint8_t BPL(); uint8_t BRK(); uint8_t BVC();
	uint8_t BVS(); uint8_t CLC(); uint8_t CLD(); uint8_t CLI();
	uint8_t CLV(); uint8_t CMP(); uint8_t CPX(); uint8_t CPY();
	uint8_t DEC(); uint8_t DEX(); uint8_t DEY(); uint8_t EOR();
	uint8_t INC(); uint8_t INX(); uint8_t INY(); uint8_t JMP();
	uint8_t JSR(); uint8_t LDA(); uint8_t LDX(); uint8_t LDY();
	uint8_t LSR(); uint8_t NOP(); uint8_t ORA(); uint8_t PHA();
	uint8_t PHP(); uint8_t PLA(); uint8_t PLP(); uint8_t ROL();
	uint8_t ROR(); uint8_t RTI(); uint8_t RTS(); uint8_t SBC();
	uint8_t SEC(); uint8_t SED(); uint8_t SEI(); uint8_t STA();
	uint8_t STX(); uint8_t STY(); uint8_t TAX(); uint8_t TAY();
	uint8_t TSX(); uint8_t TXA(); uint8_t TXS(); uint8_t TYA();

	//Illegal Opcode
	uint8_t XXX();

	void Clock();
	void Reset();
	void Irq();
	void Nmi();

	uint8_t Fetch();
	uint8_t m_Fetched = 0x00;

	uint16_t m_AddrAbs = 0x0000;
	uint16_t m_AddrRel = 0x00;
	uint8_t m_Opcode = 0x00;
	//Num cyles left for instruction
	uint8_t m_Cycles = 0;

public:
	uint8_t m_A = 0x00; //Accumulator Registor
	uint8_t m_X = 0x00; //X Registor
	uint8_t m_Y = 0x00; //Y Registor
	uint8_t m_Stkp = 0x00; //Stack Pointer
	uint8_t m_Status = 0x00; //Status Register
	uint16_t m_Pc = 0x0000; //ProgramCounter

private:
	Bus* m_Bus = nullptr;
	uint8_t Read(uint16_t addr);
	void Write(uint16_t addr, uint8_t data);

	uint8_t GetFlag(FLAGS6502 f);
	void SetFlag(FLAGS6502 f, bool v);

	struct INSTRUCTION {
		std::string name;
		uint8_t(Virtual6502::*Operate)(void) = nullptr;
		uint8_t(Virtual6502::*Addrmode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	std::vector<INSTRUCTION> m_Lookup;
};
