# Disassembler
The project implements the disassemble of the RISCV32IC instrucrtions from binary to actual text instructions.

## Implementation

The main compoment in the project is the class `Instruction` that represents the RISCV instruction, and decodes the binary instruction word given from the binary file.
* The program starts in the `main` function (main.cpp) where it reads the file and reads the instruction words from the file and stops once the instruction word is detected to be exactly equal to 0 (This is because all the memory is inialized with 0 in the beginning explicitly).
* With every 32bits read, a new instruction is constructed and placed in a vector. In the constructor of the class, it determines whether it is Compressed or not using the difference in Opcodes between non compressed (32bit) and compressed (16bit) instructions (starts with 0b11 and the latter starts with otherwise).
  * According to whether it is compressed or not, the Program Counter (PC) advances by the size of the instruction, 2 and 4 respectively.
  * Inside the Constructor, a function named `MakeInstruction()` is responsible for formatting the instruction itself and extracting the components of the instruction such as the Opcode, rs1, rs2, rd, funct3, funct7 (for 32bit instrucitons), and the Immediate values.
* Labels are added to both Branch instructions (B-Type) and Jump Instructions (J-type).
  * They are known through a flag we set inside the `MakeInstruction()` function, and in the `main` function we go through all the instructions and check whether the current instruction is a branch or jump instruction through the function `Instruction::IsBranchOrJumpInst()`. We then compute the relative offset (J_imm OR B_imm). We check whether it compressed or not in order to know the step we will take for either increasing or decreasing the relative offset. If the offset is Positive then we decrease by the size of the instruction and vice versa.
  * We then add the labels to both the branch instruction and before the destination instruction, and inside the B/J instruction.
* We go through all the instructions in the vector and start printing them one by one after adding the labels.

## Limitations

* There are some instructions in RISCV32IC that we did not implement nor included its opcode, such as `FENCE`, `EBREAK`, etc.. (Not required).
* We could not test the program on All Compressed instructions. (The tested compressed instructions are the arithmatic ones only).

## How to run

1. First download the Compressed file from the Release section, and decompress the file to your destination.  
2. Open a Terminal and run the command `rvcdiss <machine_code_file_name>` (It takes the file you want to decode as a second command line argument)

## Team Contributions:
* **[Seif Sallam](seif_sallam@aucegypt.edu)** (Team Leader): Added the base structure of the Instruction class, and contributed to adding the label functionality in the program and did the I and B instructions (Conmpressed and Non Compressed). 
* **[Yara Ali](yarayahia@aucegypt.edu)** (Member): Testing the code and formatted the U and S instructions (Compresed and non Compressed).
* **[Kareem Amr](kareemamr213@aucegypt.edu)** (Member): Testing the code and formatted the R and J instructions (Compressed and non Compressed). 

##### N.B.:
Each one of us had consistent work and contribution with the github repository as we all were working on differnet branches and merging them at different points in time such as:
1. First Merge after adding the first 32bit instructions (Branches are now deleted).
2. Second Merge after miagrating the code to be Object Oriented (Adding the Instruction class).
3. Third Merge after adding the Compressed instructions (Branches still exist but in a very old version).
4. All the final work was done inside the `master` branch.