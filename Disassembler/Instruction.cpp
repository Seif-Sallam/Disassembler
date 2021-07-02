#include "Instruction.h"

unsigned int Instruction::m_LabelCounter = 0;

Instruction::Instruction(unsigned int instructionWord, unsigned int* pc)
	: m_InstructionStr(""), m_InstructionWord(instructionWord), m_PC(pc)
{
	unsigned int checkCompression = m_InstructionWord & 0x3;
	m_IsCompressed = (checkCompression != 0x3);
	m_IsBranchOrJumpInst = false;
	if (m_PC == nullptr)
	{
		std::cout << "PC IS NULLPTR\n";
		exit(1);
	}
	MakeInstruction();
}

void Instruction::AddJumpLabel(const std::string& labelName)
{
	if(m_IsBranchOrJumpInst)
		m_LabelName = labelName;
}

void Instruction::MakeInstruction()
{
	if (m_IsCompressed)
	{
		*m_PC += 2;
		unsigned int rd, rs1, rs2, funct3, opcode;
		unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
		unsigned int address;

		unsigned int instPC = *m_PC - 2;
		opcode = m_InstructionWord & 0x3;
		funct3 = (m_InstructionWord >> 13) & 0x7;

		//Calculuate the opcode rd, rs1, rs2 and all of that 
		// I cannot write it in the very beginning because it varies from one instruction to another (See the table)
		addPrefix(instPC);
		std::stringstream ss;
		U_imm = (((m_InstructionWord >> 2) & 0x1F) << 12) | (((m_InstructionWord >> 12) & 0x1) ? 0xFFFF0000: 0x0);

		//Put in your switch statement.
		if (opcode == 0b00)
		{
			switch (funct3)
			{
			case 0b110: {
				S_imm = (((m_InstructionWord >> 10) & 0x7) << 3) | (((m_InstructionWord >> 6) & 0x1) << 2) | (((m_InstructionWord >> 5) & 0x1) ? 0xFFFFFFC0 : 0x0);
				rs1 = (m_InstructionWord >> 7) & 0x7;
				rs2 = (m_InstructionWord >> 2) & 0x7;
				ss << "\tSW\t" << getABIName(rs2) << ", " << std::hex << "0x" << (int)S_imm << "(" << getABIName(rs1) << ")\n";
			}
				break;
			case 0b010:
			{
				I_imm = (((m_InstructionWord >> 10) & 0x7) << 3) | (((m_InstructionWord >> 6) & 0x1) << 2) | (((m_InstructionWord >> 5) & 0x1) ? 0xFFFFFFC0 : 0x0);
				rs1 = (m_InstructionWord >> 7) & 0x7;
				rd = (m_InstructionWord >> 2) & 0x7;
				ss << "\tLW\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)I_imm << "(" << getABIName(rs1) << ")\n";
				break;
			}
			default:
				ss << "\tUnknown 00 Compressed Instruction\n";
			}
		}
		else if (opcode == 0b01)
		{
			switch (funct3)
			{
			case 0:
			{
				I_imm = ((m_InstructionWord >> 2) & 0x1F) | ((m_InstructionWord >> 12) ? 0xFFFFFFF0 : 0x0);
				rs1 = (m_InstructionWord >> 7) & 0x1F;
				rd = rs1;
				ss << "\tADDI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				break;
			}
			case 1: 
			{
				J_imm = (((m_InstructionWord >> 2) & 1) << 5) | (((m_InstructionWord >> 3) & 7) << 1) | (((m_InstructionWord >> 6) & 1) << 7) | (((m_InstructionWord >> 7) & 1) << 6) |
					(((m_InstructionWord >> 8) & 1) << 10) | (((m_InstructionWord >> 9) & 3) << 8) | (((m_InstructionWord >> 11) & 1) << 4) | (((m_InstructionWord >> 12) & 1) << 11) |
					(((m_InstructionWord >> 15) & 1) ? 0xFFFFFC00 : 0x0);  //change here
				rd = (m_InstructionWord >> 7) & 7;
				ss << "\tJAL\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)J_imm;
				m_IsBranchOrJumpInst = true;
				m_Offset = (int)J_imm;
				break;
			}
			case 0b011:
			{
				rd = (m_InstructionWord >> 7) & 0x1F;
				ss << "\tLUI\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)U_imm << "\n";
			}
			case 4: {
				unsigned int checkingInt = (m_InstructionWord >> 10) & 0x3;
				switch (checkingInt)
				{
				case 0x0: {	

					I_imm = ((m_InstructionWord >> 2) & 0x1F) | ((m_InstructionWord >> 12) ? 0xFFFFFFF0 : 0x0);
					rs1 = (m_InstructionWord >> 7) & 0x3;
					rd = rs1;
					ss << "\tSRLI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
					break;
				}
				case 0x1:
				{
					I_imm = ((m_InstructionWord >> 2) & 0x1F) | ((m_InstructionWord >> 12) ? 0xFFFFFFF0 : 0x0);
					rs1 = (m_InstructionWord >> 7) & 0x3;
					rd = rs1;
					ss << "\tSRAI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";

				}
				case 0x2:
				{
					I_imm = ((m_InstructionWord >> 2) & 0x1F) | ((m_InstructionWord >> 12) ? 0xFFFFFFC0 : 0x0);
					rs1 = (m_InstructionWord >> 7) & 0x3;
					rd = rs1;
					ss << "\tANDI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
					break;
				}
				case 0x3: {
					rs1 = (m_InstructionWord >> 7) & 7;
					rs2 = (m_InstructionWord >> 2) & 7;
					rd = (m_InstructionWord >> 7) & 7;
					unsigned int check;
					check = (m_InstructionWord >> 5) & 3;
					switch (check) {
					case 0:
						ss << "\tSUB\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n"; //change here
						break;
					case 1:
						ss << "\tXOR\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n"; //change here
						break;
					case 2:
						ss << "\tOR\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n"; //change here
						break;
					case 3:
						ss << "\tAND\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n"; //change here
						break;
					default:
						ss << "\tUnkown R Instruction \n";
					}
				}
				}
			}
			case 0b110:
			{
				B_imm = (((m_InstructionWord >> 3) & 0x3) << 1) | (((m_InstructionWord >> 10) & 0x3) << 3) | (((m_InstructionWord >> 2) & 0x1) << 5)
					| (((m_InstructionWord >> 5) & 0x3) << 6) | (((m_InstructionWord >> 12) & 0x1) ? 0xFFFFFF80 : 0x0);
				rs1 = (m_InstructionWord >> 7) & 0x7;
				rs2 = 0;

				ss << "\tBEQ\t" << getABIName(rs1) << ", " << getABIName(rs2) << ", " << std::hex << "0x" << (int)B_imm;
				m_IsBranchOrJumpInst = true;
				m_Offset = (int)B_imm;
				break;
			}
			case 0b111:
			{
				B_imm = (((m_InstructionWord >> 3) & 0x3) << 1) | (((m_InstructionWord >> 10) & 0x3) << 3) | (((m_InstructionWord >> 2) & 0x1) << 5)
					| (((m_InstructionWord >> 5) & 0x3) << 6) | (((m_InstructionWord >> 12) & 0x1) ? 0xFFFFFF80 : 0x0);
				rs1 = (m_InstructionWord >> 7) & 0x7;
				rs2 = 0;
				ss << "\tBNE\t" << getABIName(rs1) << ", " << getABIName(rs2) << ", " << std::hex << "0x" << (int)B_imm;
				m_IsBranchOrJumpInst = true;
				m_Offset = (int)B_imm;
				break;
			}
			default:
				ss << "\tUnknown 01 Compressed Instruction\n";
			}
		}
		else if (opcode == 0b10)
		{
			
			switch (funct3)
			{

			case 0b000:
			{
				I_imm = ((m_InstructionWord >> 2) & 0x1F) | ((m_InstructionWord >> 12) ? 0xFFFFFFF0 : 0x0);
				rs1 = (m_InstructionWord >> 7) & 7;
				rd = rs1;
				ss << "\tSLLI\t" << getABIName(rd) << ", " << getABIName(rs1) << ",x" << (int)I_imm << "\n";
				break;
			}
			case 0b100: 
			{
				rs1 = (m_InstructionWord >> 7) & 0x1F;
				rs2 = (m_InstructionWord >> 2) & 0x1F;
				rd = (m_InstructionWord >> 7) & 0x1F;
				if (((m_InstructionWord >> 2) & 0x1F) != 0x0)
				{
					ss << "\tADD\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				}
				else 
				{
					I_imm = ((m_InstructionWord >> 2) & 0x1F) | ((m_InstructionWord >> 12) ? 0xFFFFFFF0 : 0x0);
					ss << "\tJALR\t" << getABIName(0x0) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				}
				break;
			}
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
		J_imm = (((m_InstructionWord >> 12) & 0x000000FF) << 12) | (((m_InstructionWord >> 20) & 1) << 11) | (((m_InstructionWord >> 21) & 0x000003FF) << 1) |
			(((m_InstructionWord >> 31) & 1) ? 0xFFF00000 : 0x0);
		// — inst[31] — inst[30:25] inst[24:21] inst[20]
		I_imm = ((m_InstructionWord >> 20) & 0x7FF) | (((m_InstructionWord >> 31) ? 0xFFFFF800 : 0x0));
		// Calculuating the B immediate 
		// - inst[31] -- inst[7] -- inst[30:25] -- inst[11:8] - 0
		B_imm = (((m_InstructionWord >> 7) & 0x1) << 11) | (((m_InstructionWord >> 8) & 0xF) << 1) | (((m_InstructionWord >> 25) & 0x3F) << 5) |
			((m_InstructionWord >> 31) ? 0xFFFFF000 : 0x0);
		
		S_imm = ((m_InstructionWord >> 7) & 0x1F) | (((m_InstructionWord >> 24) & 0x3F) << 5) |((m_InstructionWord >> 31) ? 0xFFFFF800 : 0x0);
		U_imm = (((m_InstructionWord >> 12)) << 12);
		addPrefix(instPC);
		std::stringstream ss;
		if (opcode == 0b1110011)
		{
			ss << "\tECALL\t" << "\n";
		}
		else if (opcode == 0x33) {// R Instructions
			funct7 = (m_InstructionWord >> 24) & 0x0000007F;
			switch (funct3) {
			case 0: if (funct7 == 32) {
				ss << "\tSUB\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
			}
				  else {
				ss << "\tADD\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
			}
				  break;
			case 1: ss << "\tSLL\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				break;
			case 2: ss << "\tSLT\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				break;
			case 3: ss << "\tSLTU\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				break;
			case 4: ss << "\tXOR\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				break;
			case 5: {
				if (funct7 == 32) {
					ss << "\tSRL\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				}
				else {
					ss << "\tSRA\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				}
				break;
			}
			case 6:
				ss << "\tOR\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				break;
			case 7:
				ss << "\tAND\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << getABIName(rs2) << "\n";
				break;
			default:
				ss << "\tUnkown R Instruction \n";
			}
		}
		else if (opcode == 0x13) {	// I instructions
			switch (funct3) {
			case 0:
				ss << "\tADDI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				break;
			case 0b010: // SLTI
				ss << "\tSLTI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				break;
			case 0b011: // SLTIU
				ss << "\tSLTIU\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				break;
			case 0b100: // XORI
				ss << "\tXORI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				break;
			case 0b110: // ORI
				ss << "\tORI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				break;
			case 0b111: // ANDI
				ss << "\tANDI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
				break;
			case 0b001: //SLLI
			{
				unsigned int shiftAmount = (m_InstructionWord >> 19) & 0b11111;
				ss << "\tSLLI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)shiftAmount << "\n";
				break;
			}
			case 0b101:
			{
				unsigned int shiftAmount = (m_InstructionWord >> 19) & 0b11111;
				unsigned int lastBits = (m_InstructionWord >> 25) & 0b1111111;
				if (lastBits == 0)
					// SRLI
					ss << "\tSRLI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)shiftAmount << "\n";
				else
					//SRAI
					ss << "\tSRAI\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)shiftAmount << "\n";
				break;
			}
			}
		}
		else if (opcode == 0b0000011) //Load instructions  (I TYPE)
		{
			switch (funct3) {
			case 0b000: //LB
				ss << "\tLB\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)I_imm << "(" << getABIName(rs1) << ")\n";
				break;
			case 0b001: //LH
				ss << "\tLH\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)I_imm << "(" << getABIName(rs1) << ")\n";
				break;
			case 0b010: //LW
				ss << "\tLW\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)I_imm << "(" << getABIName(rs1) << ")\n";
				break;
			case 0b100: // LBU
				ss << "\tLBU\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)I_imm << "(" << getABIName(rs1) << ")\n";
				break;
			case 0b101: // LHU
				ss << "\tLHU\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)I_imm << "(" << getABIName(rs1) << ")\n";
				break;
			default:
				ss << "\tUnkown Load Instruciton\n";
			}
		}
		else if (opcode == 0b1100111) //JALR instruction
		{
			ss << "\tJALR\t" << getABIName(rd) << ", " << getABIName(rs1) << ", " << std::hex << "0x" << (int)I_imm << "\n";
		}
		else if (opcode == 0b1100011)  // B-Type instructions.
		{
			m_IsBranchOrJumpInst = true;
			m_Offset = (int)B_imm;
			switch (funct3)
			{
			case 0b000: //BEQ
				ss << "\tBEQ\t" << getABIName(rs1) << ", " << getABIName(rs2) << ", " << std::hex << "0x" << (int)B_imm;
				break;
			case 0b001: //BNE
				ss << "\tBNE\t" << getABIName(rs1) << ", " << getABIName(rs2) << ", " << std::hex << "0x" << (int)B_imm;
				break;
			case 0b100: //BLT
				ss << "\tBLT\t" << getABIName(rs1) << ", " << getABIName(rs2) << ", " << std::hex << "0x" << (int)B_imm;
				break;
			case 0b101: //BGE
				ss << "\tBGE\t" << getABIName(rs1) << ", " << getABIName(rs2) << ", " << std::hex << "0x" << (int)B_imm;
				break;
			case 0b110: //BLTU
				ss << "\tBLTU\t" << getABIName(rs1) << ", " << getABIName(rs2) << ", " << std::hex << "0x" << (int)B_imm;
				break;
			case 0b111: //BGEU
				ss << "\tBGEU\t" << getABIName(rs1) << ", " << getABIName(rs2) << ", " << std::hex << "0x" << (int)B_imm;
				break;
			default:
				ss << "\tUnkown B Instruction \n"; // All of them are listed already but it is here for debugging purposes
			}
		}
		else if (opcode == 0x6F)  // J instructions
		{
		ss << "\tJAL\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)J_imm;
			m_IsBranchOrJumpInst = true;
			m_Offset = (int)J_imm;
		}
		else if (opcode == 0b0100011)// S instruction
		{
			switch (funct3)
			{
			case 0b000://SB
				ss << "\tSB\t" << getABIName(rs2) << ", " << std::hex << "0x" << (int)S_imm << "(" << getABIName(rs1) << ")\n";
				break;
			case 0b001: //SH
				ss << "\tSH\t" << getABIName(rs2) << ", " << std::hex << "0x" << (int)S_imm << "(" << getABIName(rs1) << ")\n";
				break;
			case 0b010: //SW
				ss << "\tSW\t" << getABIName(rs2) << ", " << std::hex << "0x" << (int)S_imm << "(" << getABIName(rs1) << ")\n";
				break;
			default:
				ss << "\tUnknown S Instruction\n";
			}
		}
		else if (opcode == 0x37)// U type
		{
			ss << "\tLUI\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)U_imm << "\n";
		}
		else if (opcode == 0x17)
		{
			ss << "\tAUIPC\t" << getABIName(rd) << ", " << std::hex << "0x" << (int)U_imm << "\n";
		}
		else 
		{
			ss << "\tUnkown Instruction \n";
		}

		m_InstructionStr += ss.str();
	}
}
void Instruction::AddLabel(std::string labelName)
{
	labelName += ":";
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

std::string Instruction::getABIName(unsigned int regNumber)
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
	if (instruction.IsBranchOrJumpInst())
	{
		stream << instruction.GetInstructionStr() << " <" << instruction.m_LabelName << ">\n";
	}
	else
	{
		stream << instruction.GetInstructionStr();
	}
	return stream;
}
