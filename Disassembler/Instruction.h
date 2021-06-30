#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

class Instruction {
public:
	Instruction(unsigned int instructionWord, unsigned int* pc);
	void AddLabel(std::string labelName);
	inline std::string GetInstructionStr() { return m_InstructionStr; }
	inline bool IsCompressed() { return m_IsCompressed; }
	inline bool IsBranchOrJumpInst() { return m_IsBranchOrJumpInst; }
	inline static int GetNextLabelNumber() { return m_LabelCounter++; }
	inline int GetOffset() { return m_Offset; }
	void AddJumpLabel(const std::string& labelName);
	friend std::ostream& operator<<(std::ostream& stream, Instruction& instruction);
private:
	void MakeInstruction();
	void addPrefix(unsigned int instA);
	std::string getABIName(unsigned int regNumber);

	unsigned int* m_PC;
	std::string m_InstructionStr;
	std::string m_LabelName;
	bool m_IsBranchOrJumpInst;
	bool m_IsCompressed;
	int m_Offset;
	unsigned int m_InstructionWord;

	static unsigned int m_LabelCounter;
};
