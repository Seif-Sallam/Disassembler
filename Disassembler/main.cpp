#include <bitset>
#include <sstream>
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include "Instruction.h"
#include <vector>
char memory[8 * 1024];	// only 8KB of memory located at address 0
unsigned int pc = 0x0;

void emitError(const char* s)
{
	std::cout << s;
	exit(0);
}


int main(int argc, char* argv[]) {

	unsigned int instWord = 0;
	std::ifstream inFile;
	std::ofstream outFile;

	if (argc < 2) emitError("use: rvcdiss <machine_code_file_name>\n");

	inFile.open(argv[1], std::ios::in | std::ios::binary | std::ios::ate);

	std::vector<Instruction*> instructions;
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
			if (instWord == 0)
				break;
			instructions.push_back(new Instruction(instWord, &pc));
		}
	}
	else emitError("Cannot access input file\n");
	for (int i = 0; i < instructions.size(); i++)
	{
		Instruction& inst = *instructions[i];
		
		if (inst.IsBranchOrJumpInst())
		{
			int j = i;
			int offset = inst.GetOffset();
			while (offset != 0)
			{
				if (instructions[j]->IsCompressed())
				{
					if (offset > 0)
					{
						j++;
						offset -= 2;
					}
					else
					{
						j--;
						offset += 2;
					}
				}
				else
				{
					if (offset > 0)
					{
						j++;
						offset -= 4;
					}
					else
					{
						j--;
						offset += 4;
					}
				}
			}
			int labelNum = instructions[i]->GetNextLabelNumber();
			std::string labelName = "Label" + std::to_string(labelNum);
			instructions[j]->AddLabel(labelName);
			instructions[i]->AddJumpLabel(labelName);
		}

	}

	for (auto& i : instructions)
	{
		std::cout << *i;
	}
}
