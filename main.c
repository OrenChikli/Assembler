/*
 * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * This is the main of the entire program for starting it.
 *
 * the main method accepts command line arguments specifying the names of the assembly files to be encoded.
 *
 * This is the first analysis of the input, it checks if the program was given file names, and then iteratively
 * tries to open each file via the my_open method, defined in the FileFunctions.c file.
 *
 * If the files are valid, they are passed to the AssemblerActivate method found in the Assembler.c file
 * for further proceeding.
 *
 * The AssemblerActive returns a flag indicating assembly success or failure.
 * The main prints an appropriate message for the assembly success and moves to the next file.
 *
 * After all the files were scanned a message is given specifying the termination of the program
 *
 *In the Following methods and files, an errors list is created only for syntax related issues, other errors such as
 * memory allocation problems in any of the data structures will cause an error message to be displayed and the whole
 * program will be terminated.
 *
 *
 *  Algorithmic description of the project:
 * -----------------------------
 *  The file is scanned, each row at a time.
 *  The row is analysed, and if no syntax errors are discovered, one or more nodes of a list data structure called
 *  FirstPassList (defined in the FirstPassList.c file) are created, containing flags derived from the row, used for
 *  later encoding. Each node corresponds to a "word" as described in the instructions manual.
 *  If the row is a command row- the appropriate number of nodes is created and inserted in creation order to another
 *  data structure, called InstructionsList (defined in the InstructionsList.c file),
 *  where each node is inserted with the current Instructions Counter (IC) which is incremented each time.
 *
 *  If during the analysis of the row a valid symbol is found, if it is the first encounter with the symbol,
 *  it is inserted into another data structure called SymbolTable (found in SymbolTable.c) which is a red black binary
 *  search tree, ordered by the symbols string value (as determined by the strcmp method).
 *  If the symbol was previously found and this encounter is the definition of the label,  and its details are updated
 *  in the table.
 *
 *  If a valid .data directive is found, the numerical values are inserted into a list data structure called
 *  Datalist  (found in Datalist.c) as a 14 bit sized integers for later use with the address as a Data counter (DC).
 *  Similarly, if   a .string directive is found, each character in the string is imputed into the data list as well.
 *
 *  if an .entry directive is found , the corresponding symbol is inserted into a  list data structure called EntryList
 *  (found in EntryList.c) with the current IC which is later updated.
 *
 *   If after the initial scan error are found, the scanning of the current line stops and a node in another list data
 *   structure, called ErrorList (found in ErrorList.c), is formed. The node contains an appropriate Error message and
 *   the line number in which it occurred.
 *  It is possible to produce a warning message in the errors list which will be later displayed, but the row is valid
 *  and is scanned.
 *
 *  After all the rows are scanned, the symbols of any .data, .string directives as well as the directive entries
 *  themselves are updated to the correct address which is (final_IC+current_DC) for each node.
 *
 *  After the addresses are updated, the symbols table is scanned for any undefined symbols, these are symbols not given
 *  in an .external directive or not given at the beginning of a row in the form of "LABEL: ".
 *
 *  if any undefined symbols are found, the file analysis is stopped, the errors list is printed out and the next file
 *  (if given) is scanned.
 *
 *
 *  After this "firstPass" on the file, the program preforms a "SecondPass" on the first_pass_data list, and converts
 *  it to another data structure list called InstructionsList in which a correct representation of the binary code of
 *  each instruction/ data node is created.
 *
 *  If the total number of words, given by IC-(The Initial IC)+DC- (The Initial DC) is greater than the maximal allowed
 *  file size, the scaning stops and an error is added tothe errors list
 *
 * Now at this second pass, the symbols table is complete, and the external labels are known. Thus, if a a current node
 * contains this symbol- it is inserted into a list data structure called ExterList (found in ExternList.c) with the
 * correct word address within.
 *
 * At the end of both scans, if now syntax erros were found, there sould be the following:
 * An Instructions list- with the word for each instruction
 * A data list- with words for each .data/.string entry
 * An Externals list (if any were found)
 * An Entry list (if any were found)
 * An errors list with only warning or no list at all
 *
 * The assembler than prints the requested .ob, .ext, .ent files and moves to the next file.
 *
 *
 *
 *
 *
 * */


#include "FileFunctions.h" /*this header file contains declarations relevent for file manipulation*/
#include "GlobalDeclarations.h"/*This file contain declarations relevent to all the files in the program*/
#include "Assembler.h"

int main(int argc, char** argv)
{
    int i; /*a cursor to for usage in the for loop */
    FILE * fp; /*a file pointer of the current file beaing read*/
    if (argc == 1) /*only the function name was inputted in the command line*/
    {
        fprintf(stderr,"error: Missing arguments.\n");
        exit(1);
    }

    for (i=1; i<argc;i++) /*Scans the given files names stored in argv array*/
    {
        fp = my_open(argv[i], "r",as_EXTENTION);/*use a user defined method to first check and open the given file name*/
        if (fp)
        {
           if(AssemblerActivate(fp, argv[i])==TRUE) /*procces the file and print the sucses result*/
               printf("Assembly success for file: \"%s\"\n",argv[i]);
           else
               printf("Assembly failure for file: \"%s\"\n",argv[i]);
           fclose(fp);/*close the file before the next iteration*/
        }
    }
    printf("Program end.\n");
    return 0;
}