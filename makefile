ASSEMBLER: main.o Assembler.o DataList.o EntryList.o ErrorList.o ExternList.o FirstPass.o FileFunctions.o FirstPassHandlers.o FirstPassList.o InstructionsList.o SecondPass.o SymbolTable.o
	gcc -g -Wall -pedantic -ansi main.o Assembler.o DataList.o EntryList.o ErrorList.o ExternList.o FirstPass.o FileFunctions.o FirstPassHandlers.o FirstPassList.o InstructionsList.o SecondPass.o SymbolTable.o -o ASSEMBLER


main.o:	main.c
	gcc -c -Wall -ansi -pedantic main.c -o main.o

Assembler.o: Assembler.c 
	gcc -c -Wall -ansi -pedantic Assembler.c -o Assembler.o

DataList.o: DataList.c 
	gcc -c -Wall -ansi -pedantic DataList.c -o DataList.o

EntryList.o: EntryList.c 
	gcc -c -Wall -ansi -pedantic EntryList.c -o EntryList.o

ErrorList.o: ErrorList.c 
	gcc -c -Wall -ansi -pedantic ErrorList.c -o ErrorList.o

ExternList.o: ExternList.c 
	gcc -c -Wall -ansi -pedantic ExternList.c -o ExternList.o

FileFunctions.o: FileFunctions.c 
	gcc -c -Wall -ansi -pedantic FileFunctions.c -o FileFunctions.o

FirstPass.o: FirstPass.c 
	gcc -c -Wall -ansi -pedantic FirstPass.c -o FirstPass.o

FirstPassHandlers.o: FirstPassHandlers.c 
	gcc -c -Wall -ansi -pedantic FirstPassHandlers.c -o FirstPassHandlers.o

FirstPassList.o: FirstPassList.c 
	gcc -c -Wall -ansi -pedantic FirstPassList.c -o FirstPassList.o

InstructionsList.o: InstructionsList.c 
	gcc -c -Wall -ansi -pedantic InstructionsList.c -o InstructionsList.o
	
SecondPass.o: SecondPass.c 
	gcc -c -Wall -ansi -pedantic SecondPass.c -o SecondPass.o
	
SymbolTable.o: SymbolTable.c 
	gcc -c -Wall -ansi -pedantic SymbolTable.c -o SymbolTable.o
