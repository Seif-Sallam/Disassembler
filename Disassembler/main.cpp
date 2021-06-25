#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>

using namespace std;

unsigned int pc = 0x0;

char memory[8 * 1024];	// only 8KB of memory located at address 0

void emitError(const char* s)
{
	cout << s;
	exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW) {
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}

void instDecExec(unsigned int instWord)
{
	unsigned int rd, rs1, rs2, funct3, funct7, opcode, imm1to10, imm11, imm12to19, imm20, imm0, imm;
	unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;

	unsigned int instPC = pc - 4;

	opcode = instWord & 0x0000007F;
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;
	funct7 = (instWord >> 25) & 0x0000007F;
	// — inst[31] — inst[30:25] inst[24:21] inst[20]
	J_imm = (((instWord >> 12) & 0x000000FF) << 12) | (((instWord >> 20) & 1) << 11) | (((instWord >> 21) & 0x000003FF) << 1) |
		((instWord >> 31) ? 0xFFFFF800 : 0x0);

	printPrefix(instPC, instWord);

	if (opcode == 0x33) {		// R Instructions
		switch (funct3) {
		case 0: if (funct7 == 32) {
			cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
		}
			  else {
			cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
		}
			  break;
		case 1: cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		case 2: cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		case 3: cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		case 4: cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		case 5: if (funct7 == 32) {
			cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
		}
			  else {
			cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
		}
			  break;
		case 6: cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		case 7: cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		default:
			cout << "\tUnkown R Instruction \n";
		}
	}

	else if (opcode == 0x6F)
	{
		cout << "\tJAL\tx" << rd << ", " << hex << "0x" << (int)J_imm << "\n";
	}
	else {
		cout << "\tUnkown J Instruction \n";

	}

}

int main(int argc, char* argv[]) {

	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;



	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		int fsize = inFile.tellg();

		inFile.seekg(0, inFile.beg);
		if (!inFile.read((char*)memory, fsize)) emitError("Cannot read from input file\n");

		while (true) {
			instWord = (unsigned char)memory[pc] |
				(((unsigned char)memory[pc + 1]) << 8) |
				(((unsigned char)memory[pc + 2]) << 16) |
				(((unsigned char)memory[pc + 3]) << 24);
			pc += 4;
			// remove the following line once you have a complete simulator
			if (pc == 40) break;			// stop when PC reached address 32
			instDecExec(instWord);
		}
	}
	else emitError("Cannot access input file\n");
}