# incomplete
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

 - The CPU has 8 registers:  r<sub>0</sub> ,r<sub>1</sub> ,r<sub>2</sub> ,r<sub>3</sub> ,r<sub>4</sub> ,r<sub>5</sub> ,r<sub>6</sub> ,r<sub>7</sub>
 - The size of each register is 14 bits. The LSB (Least significate Bit) is bit no. 0 and the MSB (Most Significant bit) is bit no. 12
 -The CPU also has an PSW (Program Status word) register. which contains a number of flags characterizing the work status of the CPU at any given moment.
 - The flags are later described
 - The size of the memory is 256 cells in addresses 0-255 (in decimal base). where every cell is 14 bit is size.
 - A cell in the memory is referred to as a Word
- character encoding is done with ascii


# Machine code comand structure
 each machine code command is encoded to several memory words in the following format:
 
 
Bit  | 0  1 | 2  3 | 4  5 | 9  8  7  6 | 10  11 | 12  13|
------------ | -------------|------------ | -------------|------------ | -------------|------------ | 
Encoding | E,R,A | Destination operdand addresing | source operand addresing | opcode | parameter<sub>2</sub> | parameter<sub>1</sub>

## Bits 01 (A,R,E)
These determine the encoding type:
- 00 Absolute 
- 01: External
- 10 : Relocatable

These are aded only for instruction code encodings (not data encoding).

## Bits 2-3
These encode the number of the destination operand

## Bits 4-5
These encode the number of the source operand

## Bits 6-9
These encode the "opcode" of the instruction from the following table:

 Code (in base 10) | Action name
------------------- | -----------
0          |          mov
1          |         cmp
2          |         add
3          |          sub
4          |          not
5          |          clr
6          |          lea
7          |          inc
8          |          dec
9          |          jmp
10         |           bne
11         |           red
12         |           prn
13         |           jsr
14         |          rts
15         |           stop


## Addressing methods
 for this project there are 4 adressing methodes, numbered 0-3 as follows:
 
 value | Addressing method |Extra Word content | Writing convention | Example 
---------------|---------------|---------------|---------------|---------------|
0     | Immidiate adressing    | The extra word contains the operand itselfm which is a number represented by 12 bits, which are copuled with a pair of A,R,E bits | The operand starts with the characther \# folowed with a decimal number | move #-1,r2
1     | Directe Addressing | The extra word contains the destination of the word in the memory. This word is the operand. The destination is represented by 12 bits copuled with the A,R,E bits | The operand is a label which was already declared, or will be later declared. The declaration is done by writining ".data" or ".string" in the beginig of the instruction or by the ".extern" instruction | for the declaration: "x:.data 23" the following instruction "dec x" can be writen
2    | jump with parameters addressing | this method is relevent only for the jump operands - jmp,bne,jsr. it is meant for the convenient passing of two parameters for usage in the jump destination. The first parameter will be passed in register r<sub>6</sub> and the other in r<sub>7</sub>. For this method there are at most 3 extra words. The first is the jump address, the second is the encoing of the first parameter and the third is the encoding of the second parameter. It is possibe that both parameters will be encoded into one word, in which case there are only 2 extra words in the comand.  | The operand in composed of the name of a label which is the jump destination, folowed in brackets are the two parameters, coma seperated (without any spaces in between). Each parameter can be an immidiate number (such as in  addressing method 0), a lael (such as in  addressing method 1), or a register (such as in  addressing method 3). The bits of each parameter in the first word are determined by the parameter type. the extra word will be encoded as the relevent addressing method. If the first parameter is a register, it will be encoded in an addtional word, such as the source register in addressing method 3. If the second parameter is a register, it will be encoded such as the destination operand. If both are registers, than they will share only one word.  | jmp L1(#5,N) here, the costant 5 is encoded in the third word of the instruction, and the address represented by the label "N" is encoded in the forth word. This instruction performs a jump to label "L1", passing two parameters: the constant 5 (whic is in register r<sub>6</sub> and the word in adress N (which is in register r<sub>7</sub>. Another example: jsr L1(r3,r5)  here both the parameters are registers and thus share the third instruction word.
3 | Direct register addressing | The operand is a register. If it is a destination register, another word will contain the register number in bits 2-7. If it is a source register, than another words will contain the register number in buts 8-13. If there are two registers, they will share one extra word, where bits 2-7 are for the destination register and bits 8-13 are for the source register. nused bits are assigned 0 | the operand is a name of a register | move r1,r2 here the command copeis the content of register r1 into register r2. Both operands are registers which will be encoded into another single word.
