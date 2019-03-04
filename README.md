# Short Description
Final project for the System Programming Laboratory course in the Open University of Israel : A two pass Assembler program written in C for a custom assembly language

# Assembler
 The purpose of the assembler is to create machine code for a file written in assembly language. 
 This is the first step in the path which the program undergoes, up until a code which is ready to run on the machine hardware is
 achieved. The following steps are the linkage and loading of the code, which are not dealt with ibn this project.
 
 # Main goal
 An assembler written in c for a custom assembly language
 
 # Project definitions
 ## "Hardware"
 - The computer in the project is composed of a CPU, registers and RAM memory, where some of the memory also acts as a stack.
 - The size of a Word in memory is 14 bits.
 - The arithmetic is done in 2's compliment
 - This computer works only with integers, there is no support for for floats.
 
 ## Registers
 - The CPU has 8 registers: r0,r1,r2,r3,r4,r5,r6,r7
 - The size of each register is 14 bits. The LSB (Least significate Bit) is bit no. 0 and the MSB (Most Significant bit) is bit no. 12
 -The CPU also has an PSW (Program Status word) register. which contains a number of flags characterizing the work status of the CPU at any given moment.
 - The flags are later described
 - The size of the memory is 256 cells in addresses 0-255 (in decimal base). where every cell is 14 bit is size.
 - A cell in the memory is referred to as a Word
- character encoding is done with ascii


 
 
# Uncomplete
