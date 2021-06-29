#include "Instruction.h"

unsigned int Instruction::m_LabelCounter = 0;

Instruction::Instruction(unsigned int instructionWord, unsigned int* pc)
	: m_InstructionStr(""), m_InstructionWord(instructionWord), m_PC(pc)
{
	unsigned int checkCompression = m_InstructionWord & 0x3;
	m_IsCompressed = (checkCompression != 0x3);
	if (m_PC == nullptr)
	{
		std::cout << "PC IS NULLPTR\n";
		exit(1);
	}
	MakeInstruction();
}

void Instruction::MakeInstruction()
{
	if (m_IsCompressed)
	{
		*m_PC += 2;
		unsigned int rd, rs1, rs2, funct3, funct7, opcode;
		unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
		unsigned int address;

		unsigned int instPC = *m_PC - 2;
		opcode = m_InstructionWord & 0x3;
		funct3 = (m_InstructionWord >> 13) & 0x7;

		//Calculuate the opcode rd, rs1, rs2 and all of that 
		// I cannot write it in the very beginning because it varies from one instruction to another (See the table)
		addPrefix(instPC);
		std::stringstream ss;
		//Put in your switch statement.
		if (opcode == 0b00)
		{
			switch (funct3)
			{
			default:
				ss << "\tUnknown 00 Compressed Instruction\n";
			}
		}
		else if (opcode == 0b01)
		{
			switch (funct3)
			{

			default:
				ss << "\tUnknown 01 Compressed Instruction\n";
			}
		}
		else if (opcode == 0b10)
		{
			switch (funct3)
			{

			default:
				ss << "\tUnknown 01 Compressed Instruction\n";
			}
		}
		else
		{
			ss << "\t Unknown Compressed Instruction\n";
		}
		m_InstructionStr += ss.str();
	}
	else
	{
		*m_PC += 4;
		unsigned int rd, rs1, rs2, funct3, funct7, opcode;
		unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
		unsigned int address;

		unsigned int instPC = *m_PC - 4;

		opcode = m_InstructionWord & 0x0000007F;
		rd = (m_InstructionWord >> 7) & 0x0000001F;
		funct3 = (m_InstructionWord >> 12) & 0x00000007;
		rs1 = (m_InstructionWord >> 15) & 0x0000001F;
		rs2 = (m_InstructionWord >> 20) & 0x0000001F;

		// — inst[31] — inst[30:25] inst[24:21] inst[20]
		I_imm = ((m_InstructionWord >> 20) & 0x7FF) | (((m_InstructionWord >> 31) ? 0xFFFFF800 : 0x0));
		S_imm = (((m_InstructionWord >> 7) & 1) << 11) | (((m_InstructionWord >> 8) & 0xF) << 1) | (((m_InstructionWord >> 24) & 0x3F) << 5) |
			((m_InstructionWord >> 31) ? 0xFFFFF800 : 0x0);
		addPrefix(instPC);
		std::stringstream ss;

		if (opcode == 0x33) {// R Instructions
			funct7 = (m_InstructionWord >> 24) & 0x0000007F;
			switch (funct3) {
			case 0: if (funct7 == 32) {
				ss << "\tSUB\t" << getAPIName(rd) << ", " << getAPIName(rs1) << ", " << getAPIName(rs2) << "\n";
			}
				  else {
				ss << "\tADD\t" << getAPIName(rd) << ", " << getAPIName(rs1) << ", " << getAPIName(rs2) << "\n";
			}
				  break;
			default:
				ss << "\tUnkown R Instruction \n";
			}
		}
		else if (opcode == 0x13) {	// I instructions
			switch (funct3) {
			case 0: ss << "\tADDI\t" << getAPIName(rd) << ", " << getAPIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				break;
			default:
				ss << "\tUnkown I Instruction \n";
			}
		}
		else if (opcode == 0b0100011)// S instruction
		{
			switch (funct3)
			{
			case 0b000://SB
				ss << "\tSB\t" << getAPIName(rs1) << ", " << getAPIName(rs2) << ", " << std::hex << "0x" << (int)S_imm << "\n";
				break;
			case 0b001: //SH
				ss << "\tSH\t" << getAPIName(rs1) << ", x" << getAPIName(rs2) << ", " << std::hex << "0x" << (int)S_imm << "\n";
				break;
			case 0b010: //SW
				ss << "\tSW\t" << getAPIName(rs1) << ", x" << getAPIName(rs2) << ", " << std::hex << "0x" << (int)S_imm << "\n";
				break;
			default:
				ss << "\tUnknown S Instruction\n";
			}
		}
		else if (opcode == 0x37)// U type
		{
			ss << "\tLUI\t" << getAPIName(rd) << ", " << std::hex << "0x" << (int)U_imm << "\n";
		}
		else if (opcode == 0x6F)
		{
			ss << "\tAUIPC\t" << getAPIName(rd) << ", " << std::hex << "0x" << (int)U_imm << "\n";
		}
		else {
			ss << "\tUnkown Instruction \n";
		}

		m_InstructionStr += ss.str();
	}
}
void Instruction::AddLabel(const std::string& labelName)
{
	for (int i = 0; i < labelName.size() + 1; i++) {
		if(i < labelName.size())
			m_InstructionStr.insert(m_InstructionStr.begin() + i, labelName[i]);
		else
			m_InstructionStr.insert(m_InstructionStr.begin() + i, '\n');
	}
}
void Instruction:: addPrefix(unsigned int instA) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << m_InstructionWord;
	m_InstructionStr = ss.str();
}

std::string Instruction::getAPIName(unsigned int regNumber)
{
	switch (regNumber)
	{
	case 0:
		return "zero";
		break;
	case 1:
		return "ra";
		break;
	case 2:
		return "sp";
		break;
	case 3:
		return "gp";
		break;
	case 4:
		return "tp";
		break;
	case 5:
		return "t0";
		break;
	case 6:
		return "t1";
		break;
	case 7:
		return "t2";
		break;
	case 8:
		return "s0";
		break;
	case 9:
		return "s1";
		break;
	case 10:
		return "a0";
		break;
	case 11:
		return "a1";
		break;
	case 12:
		return "a2";
		break;
	case 13:
		return "a3";
		break;
	case 14:
		return "a4";
		break;
	case 15:
		return "a5";
		break;
	case 16:
		return "a6";
		break;
	case 17:
		return "a7";
		break;
	case 18:
		return "s2";
		break;
	case 19:
		return "s3";
		break;
	case 20:
		return "s4";
		break;
	case 21:
		return "s5";
		break;
	case 22:
		return "s6";
		break;
	case 23:
		return "s7";
		break;
	case 24:
		return "s8";
		break;
	case 25:
		return "s9";
		break;
	case 26:
		return "s10";
		break;
	case 27:
		return "s11";
		break;
	case 28:
		return "t3";
		break;
	case 29:
		return "t4";
		break;
	case 30:
		return "t5";
		break;
	case 31:
		return "t6";
		break;
	}
}

std::ostream& operator<<(std::ostream& stream, Instruction& instruction)
{
	stream << instruction.GetInstructionStr();
	return stream;
}
