#include <fstream>
#include <vector>
#include "Instruction.h"
char memory[8 * 1024];	// only 8KB of memory located at address 0
unsigned int pc = 0x0;

void emitError(const char* s)
{
	std::cout << s;
	exit(0);
}


int main(int argc, char* argv[]) {

	memset((void*)memory, 0, 8 * 1024); // explicitly making the starting memory all 0s
	unsigned int instWord = 0;
	std::ifstream inFile;

	if (argc < 2) emitError("use: rvcdiss <machine_code_file_name>\n");

	inFile.open(argv[1], std::ios::in | std::ios::binary | std::ios::ate);

	//Instructions vector that will contain all the instructions in this current program
	std::vector<Instruction*> instructions;
	if (inFile.is_open())
	{

		//finding the size of the file
		int fsize = inFile.tellg();
		
		inFile.seekg(0, inFile.beg);
		if (!inFile.read((char*)memory, fsize)) emitError("Cannot read from input file\n");

		while (true) {

			//extracting 32 bits for the instruction word by a sequence of shifting and ORing between bytes of memory
			instWord = (unsigned char)memory[pc] |
				(((unsigned char)memory[pc + 1]) << 8) |
				(((unsigned char)memory[pc + 2]) << 16) |
				(((unsigned char)memory[pc + 3]) << 24);

			//The whole file will be done once the instruction word is ALL 0s (this is already known because 
			//the memory we have is inialized with all 0s as we explicitly said so, and if it happens that there is 
			//an actual instruction that is all 0s, then it is known to be illegel for RISC-V)
			if (instWord == 0)
				break;

			// pushing the new instruction we made into the instructions vector
			instructions.push_back(new Instruction(instWord, &pc));

			//We do not increment the program counter (pc) here because it already does in the instruction class
			//*A design specific idea*
		}
	}
	else emitError("Cannot access input file\n");

	//A loop to go through the instructions one by one and see if it is a jump or branch instruction
	// to add the labels before a specific instruction and inside the B and J instructions
	for (int i = 0; i < instructions.size(); i++)
	{
		Instruction& inst = *instructions[i];
		
		//Making sure it is a branch or jump instruction
		if (inst.IsBranchOrJumpInst())
		{
			int j = i;
			//To know the offset of the B or J instruction (the number of moves i need to take)
			int offset = inst.GetOffset(); 

			//Keep moving until we reach the needed instruction
			while (offset != 0)
			{
				//Making sure that if it is compressed I move by a step of 2 (Half Word)else i move by a step of 4(Word)
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
			}//End of While
			std::string labelName;
			if (instructions[j]->HasLabel(labelName))
			{
				instructions[i]->AddJumpLabel(labelName);
			}
			else
			{
				//Finding the label Number and adding the label.
				labelName = "Label" + std::to_string(Instruction::GetNextLabelNumber());
				//j inductive variable used to indicate the index of the destination instruction
				instructions[j]->AddLabel(labelName);
				//i indeuctive variable used to indicate the index of the branch or jump instruction (source)
				instructions[i]->AddJumpLabel(labelName);
			}
		}

	}

	// very simple iterator loop to print the instructions
	for (auto& i : instructions)
		std::cout << *i;

	for (int i = 0; i < instructions.size(); i++)
	{
		delete instructions[i];
	}
	//End of main
	return 0;
}
