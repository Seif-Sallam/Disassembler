#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>



class Instruction {
public:
	Instruction(unsigned int instructionWord, unsigned int* pc);
	void AddLabel(const std::string& labelName);
	inline std::string GetInstructionStr() { return m_InstructionStr; }
private:
	void MakeInstruction();
	void addPrefix(unsigned int instA);
	std::string getAPIName(unsigned int regNumber);
	unsigned int* m_PC;
	std::string m_InstructionStr;
	unsigned int m_InstructionWord;
	bool m_IsCompressed;
};

std::ostream& operator<<(std::ostream& stream, Instruction& instruction);