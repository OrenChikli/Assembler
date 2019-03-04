
/*
 *
 *  * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 * FINAL PROJECT
 * ===========
 *
 * FileFunctions.c
 *
 * Short Description:
 * ------------------
 *A file defining methods for file manipulation: file opening and file creation.
 *
 * ----------------------------------------
 */
#include "FileFunctions.h"

#include "ExternList.h"
#include "EntryList.h"
#include "DataList.h"
#include "InstructionsList.h"
#include "ErrorList.h"

#define NUM_OF_EXTENTIONS 4 /*the number of possible file extentions as defined in the project*/
#define WORD_SIZE 14 /*The bit size of an assembly word (used in the print_weird() method*/

char* FILE_EXTENTIONS[NUM_OF_EXTENTIONS]={".as",".ob",".ext",".ent"};
/*An array of strings containing a string of a file extensions, in the order corresponding the the
 * FILE_EXTENTIONS_ENUM enumeration  in the header file */


void print_weird(FILE *object, unsigned short int x); /*an internal method for printing the .ob file in the
 * "Weird base" as defined in p15 of the instructions manual*/

/*
 * my_open():
 * -------------
 * a method for opening files given by their names- the input char* name
 * this is a modification of the fopen method, similar syntax is used- the input 'char *mode' for specifying the
 * opening mode as for the fopen function. e.g "r","w", e.t.c
 *
 * the method checks for valid files, and prints an appropriate error for any problem
 *
 *
 *
 * */


FILE *my_open(char *name, char *mode, int extension)
{
    FILE *fp; /*a file pointer for openin the given file name in *name*/
    char extended_name[MAX_LINE_SIZE]; /*s string for storing the file name with the extentions,
 *                                        e.g for given name of "test"  with extention .as
 *                                        extended_name will hold "test.as"*/
    if (strchr(name,'.')) /*cheking that a valid file name was given without an extention*/
    {
        fprintf(stderr,"Error opening %s: must input file names without extensions \n",name);
        return NULL;
    }

    strcpy(extended_name,name);/*move the file name into extended_name*/
    strcat(extended_name,FILE_EXTENTIONS[extension]); /*add the extention to to file name*/

    fp = fopen(extended_name,mode); /*open the file*/

    if(fp == NULL) /*error opening the file*/
    {
        fprintf(stderr,"Cannot open the file \"%s\".\n",name);
    }

    else if (mode[0]=='r') /*if the file is being read*/
    {
        long len; /*an integer for length of the file*/
        fseek(fp, 0, SEEK_END); /*go to the end of the file*/
        len =  ftell(fp); /*get the distance from the beginning of the file */
        if (len == 0) /*the file is empty*/
        {
            fprintf(stderr, "The file \"%s\" is empty!\n",name);
            fclose(fp);
            fp = NULL;
        }
        else/*the file isnt empty- reset the file pointer*/
            rewind(fp);/*the file is not empty-reset fp*/
    }
    return fp;
}


/*
 * GenerateOutFiles():
 * -------------
 * A method for creating the output files after the successful scan of the file and the creation of the data structures
 * containg the data to be printed, mainly the InstructionsList *defined in the InstructionsList.c file) and Datalist
 * (defined in the DataList.c file).
 * The method will print the .ob file, with the .ent and .ext file being  created if the corresponding
 * data structures are not NULL are created
 *
 * */

void GenerateOutFiles(int IC, int DC, char *name)
{
    /*File pointer for the output files*/
    FILE * ob_file = NULL;
    FILE * ent_file = NULL;
    FILE * ext_file = NULL;

    unsigned short * Word_binary_Code; /* a pointer to the numerical representation of the binary code for the word
 *                                         to ne printed, as defined in the assembler syntax*/

    instructions_list_ptr Instructions_p = NULL; /* a pointer to the instructions list for scanning each word and printing it
 * to the .ob file*/

    data_ptr Data_p = NULL;/* a pointer to the data list for scanning each word and printing it to the .ob file, if it
 * will not be null when accessed */

    entry_ptr Entry_p = NULL;/* a pointer to the entry list for scanning each entry and printing it to the .ent file,if it
 * will not be null when accessed  */

    extern_ptr Extern_p=NULL;/* a pointer to the externals list for scanning each external symbol and printing it to the
 * .ent file,if it will not be null when accessed  */

    ob_file = my_open(name, "w", ob_EXTENTION); /*use the my_open file to create the .ob file*/

    returnInstructionsHead(&Instructions_p); /*get the head of the instructions list*/


    fprintf(ob_file, "\t%d\t%d\n", (IC-INITIAL_IC), (DC-INITIAL_DC)); /*print the first line, containig the number of
 * commands and the number of data entries*/

    while (Instructions_p) /*scan the instructions list*/
    {

        Word_binary_Code= get_Word_Binary_Value(Instructions_p); /*get a pointer to the numerical word value*/

        fprintf(ob_file, "%04u\t", getInstructionAddress(Instructions_p)); /*print the Address in the .ob file,
 *                                 with a 4 digit notation*/
        print_weird(ob_file, *Word_binary_Code); /*print the representation of the word in the "Weird Base"*/


        Instructions_p = getNextInstruction(Instructions_p); /*move to the next node in the list*/
    }

    /*at this point the instruction were printed to the ob,if there are any words in the data list, they are now
     * printed*/
    returnDataHead(&Data_p); /*get the head of the Data list*/

    while (Data_p)/*scan the data list*/
    {
        fprintf(ob_file, "%04u\t", getDataAddress(Data_p));/*print the Address in the .ob file,
 *                                 with a 4 digit notation*/
        print_weird(ob_file, getDataValue(Data_p));/*print the representation of the word in the "Weird Base"*/

        Data_p= getNextData(Data_p);/*move to the next node in the list*/

    }

    /*at this point the .ob file was created. If there are any entries in the EntryList, an .ent file is created and
     * the entries are printed to it*/

    returnEntryHead(&Entry_p); /*get the head of the Entry list*/

    if (Entry_p)/*check if the list in not empty*/
    {
        ent_file = my_open(name, "w", ent_EXTENTION);/* create the .ent file*/

        while (Entry_p)/*scan the entry list*/
        {
            fprintf(ent_file, "%s %d\n", getEntrySymbol(Entry_p), getEntryAddress(Entry_p));
            /*print the address and symbol of the entry*/
            Entry_p = getNextEntry(Entry_p);/*move to the next node in the list*/
        }
    }

    /*the same as with the entry list- if the extern list is not null the  .ext file is created and filled*/

    returnExternHead(&Extern_p); /*get the head of the Extern list*/

    if (Extern_p) /*check if the list in not empty*/
    {
        ext_file = my_open(name, "w", ext_EXTENTION); /* create the .ext file*/

        while (Extern_p) /*scan the extern list*/
        {
            fprintf(ext_file, "%s %d\n", getExternSymbol(Extern_p), getExternAddress(Extern_p));
            /*print the address and symbol of the entry*/
            Extern_p = getNextExtern(Extern_p);/*move to the next node in the list*/
        }
    }

    /*if the files were created, then they must be closed*/
    if (ob_file)
        fclose(ob_file);
    if (ent_file)
        fclose(ent_file);
    if (ext_file)
        fclose(ext_file);
}


/*
 * print_weird():
 * -----------------
 * A method for printing a given numerical value in the weird base as defined in p.15 of the instructions manual
 * The method uses a mask  with only 1 "on" bit' e.g for a 3 bit number  a mask can be 100.
 * The method uses the bitwise AND operation to check the bit in the same position as the on bit is 1 or zero,
 * if it is 1, the '/' character is printed, if 0 than the '.' character is printed
 * en example:
 * for the integer 1011 the result of the AND operation with:
 *                  - The mask 1000 is 1 and thus '\' is printed.
 *                  - The mask 0100 is 0 and thus '.' is printed.
 *The mask is changed to acoount for all the bits of the number.
 *
 *
 * */

void print_weird(FILE *object, unsigned short int x)
{
    unsigned short int mask = (unsigned short)(1<<(WORD_SIZE-1)); /*the initial mask, setting  the on bit at the
 * MSU possiton. for a 14 bit sized number- the bit at position 13: 10000000000000*/
    while (mask) /*as lont as the mask is not zero*/
    {
        /*check the current bit*/
        if ((x&mask) == 0)
            fprintf(object, ".");

        else
            fprintf(object, "/");
       
        mask >>= 1;/*shift the on bit the the next position, eg. 1000 wil becom 0100*/
    }
    fprintf(object, "\n");/*the print is finished, move to the next line in the file*/
}
