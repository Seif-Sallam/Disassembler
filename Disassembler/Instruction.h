#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

/// <summary>
/// Instruction Class:
/// A class that represents a RISC-V32I instruction that has the member variables: 
/// </summary>
/// 
/// <param name="m_PC">Program counter pointer</param>
/// <param name="m_InstructionStr">Instruction String that will be printed</param>
/// <param name="m_IsBranchOrJumpInst">Boolean to tell whether it is a branch or jump instruction</param>
/// <param name="m_IsCompressed">Boolean to tell if it is Compressed or not</param>
/// <param name="m_Offset">Offset value for if the instruction is a branch instruction </param>
/// <param name="m_InstructionWord">Instruction word value itself </param>
/// 
/// <para>static integer to count the number of labels to determine the next label number (m_LabelCounter)</para>
class Instruction {
public:
	//Deleted Default constructor 
	Instruction() = delete;
	//Deleted copy constructor
	Instruction(const Instruction&) = delete;
	
	/// <param name="instructionWord">The instruction word to be given to</param>
	/// <param name="pc">Program Counter pointer (beacause it will change inside the class)</param>
	Instruction(unsigned int instructionWord, unsigned int* pc);
	
	/// <summary>
	/// function to add a label before the instruction
	/// </summary>
	/// <param name="labelName">The name of the label to be added</param>
	void AddLabel(std::string labelName);

	/// <summary>
	/// Function to get the instruction string, needed mostly for debugging.
	/// </summary>
	/// <returns>std::string: Instruction string</returns>
	inline std::string GetInstructionStr() { return m_InstructionStr; }

	/// <summary>
	/// Returns if the instruction was detected to be compressed or not 
	/// </summary>
	/// <returns>bool: IsCompressed</returns>
	inline bool IsCompressed() { return m_IsCompressed; }

	/// <summary>
	/// Returns if the instruction already has a label
	/// </summary>
	/// <returns></returns>
	inline bool HasLabel(std::string& labelName) { labelName = m_LabelName; return m_HasALabel; }

	/// <summary>
	/// Returns if the instruction was detected to be Branch or a Jump instruction to know 
	/// if we should give it a label or not
	/// </summary>
	/// <returns>bool: IsBranchOrJumpInstruction</returns>
	inline bool IsBranchOrJumpInst() { return m_IsBranchOrJumpInst; }

	/// <summary>
	/// Returns the next label number and increments the static variable
	/// </summary>
	/// <returns>int: LabelCounter</returns>
	inline static int GetNextLabelNumber() { return m_LabelCounter++; }

	/// <summary>
	/// Returns the offset that was determined from the B or J instructions
	/// </summary>
	/// <returns>int: offset</returns>
	inline int GetOffset() { return m_Offset; }

	/// <summary>
	/// Adds a label to the jump or branch instruction that is INSIDE the instruction itself
	/// </summary>
	/// <param name="labelName">the name of the label we want to add</param>
	void AddJumpLabel(const std::string& labelName);

	/// <summary>
	/// A friend operator overloading to the output stream to the left shift operator to make it easier 
	/// for us to print the instruction string
	/// </summary>
	/// <param name="stream">output stream</param>
	/// <param name="instruction"> a reference to the instruction we want to print</param>
	/// <returns>the stream</returns>
	friend std::ostream& operator<<(std::ostream& stream, Instruction& instruction);
private:

	/// <summary>
	/// The core function of the instruction, it does everything to the instruction.
	/// <para>It determines what the opcode is and the different elements of the instruction and then
	/// goes through a sequence of switch/if statements to get the correct combination of codes to get the final
	/// instruction string.</para>
	/// </summary>
	void MakeInstruction();

	void MakeBInstruction();
	void MakeRInstruction();
	void MakeIInstruction();
	void MakeSInstruction();
	void MakeUInstruction();
	void MakeLoadInstruction();
	void Make00Instruction();
	void Make01Instruction();
	void Make10Instruction();

	/// <summary>
	/// Helper function to add a prefix to the instruction string such as the word value and the address
	/// </summary>
	/// <param name="instA">The instruction address</param>
	void addPrefix(unsigned int instA);

	/// <summary>
	/// Helper function that returns the ABI name of the registers.
	/// </summary>
	/// <param name="regNumber">The number of the register</param>
	/// <returns>The string of that register</returns>
	std::string getABIName(unsigned int regNumber);

private:
	unsigned int* m_PC;
	std::string m_InstructionStr;
	std::string m_LabelName;
	bool m_IsBranchOrJumpInst;
	bool m_IsCompressed;
	bool m_HasALabel;
	int m_Offset;
	unsigned int m_InstructionWord;
	static unsigned int m_LabelCounter;
};
