
/*
 *  * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 *
 *
 FINAL PROJECT
===========

InstructionsList.c

Short Description:
------------------
 * A file defining the InstructionsList data structure, which is a singly liked list for containing a representation
 * of a word in assembly syntax, derived form a command in the .as file
 * The words list is a list of all the words scanned - a word is a union of 3 types of words depending on
 * their addressing.
 *
 * FirstWord- a structure defining the first word of every instruction
 * ImmediateWord- a structure defining a word for Immediate and labels since thier encoding is similar
 * RegisterWord- a structure defining a word for Registers.
 *
 *
 * The insertion to this list is done in a non changing order, meaning each nodes location is final and
 * thus a Tail node is created for better time complexity insertion at the end of the list
 *
 *Assumptions:
 * The first pass was completed successfully and each node in the first_pass_list node is valid
 * and the symbols table is complete.
 *
 * */


#include "InstructionsList.h"
#include "GlobalDeclarations.h"
#include "SymbolTable.h"
#include "ExternList.h"

/*Internal method declarations*/

enum Operand_TYPE {SOURCE_OPERAND,DEST_OPERAND} ;
/*An enumeration to distinguish between source Operands and DEstination operans, used by set_register_word method */

enum  ENCODING {ABSOLUTE_ENCODING,EXTERNAL_ENCODING,RELOCATABLE_ENCODING};
/*an enumeration of the ERA bit addreesing for assembly words as described in p16 of the instructions manuall
  * e.g IABSOLUTE_ENCODING is 0 as specified in the manual, and so on*/

enum WORD_NUMBER {FIRST_WORD,IMMEDIATE_WORD,REGISTER_WORD};
/*An enumeration to distinguish between Word types. IN this program, there are three types of structs representing an
 * assembler word as described in  p16 of the instruction manual.*/


void Set_Word(int Command_Operand_Flag,int Jump_Operand_flag, instructions_list_ptr Current, int Value, int IC, char *str);
/*a method for setting a the values of a given word, which is not a register word */

void set_register_word( instructions_list_ptr Current,int OperandType, int Value);
/*a method for setting a the values of a given register type word*/

void insertToWordsList(instructions_list_ptr node); /*insert a create node into the list*/

instructions_list_ptr InstuctionNodeAllocate(int address); /* a method for allocating space for a node*/

/* definition for the first word of each instruction- contains  the fields defined in p15 of the assignment*/
typedef struct {

    unsigned int ERA: 2; /*bits of the ERA field*/
    unsigned int DestAddressing: 2;/* Bit mapping for the Addressing method of the Destination Operand*/
    unsigned int SourceAddressing: 2; /* Bit mapping for the Addressing method of the Source Operand*/
    unsigned int Opcode: 4;/*Bit Mapping of the OPCODE*/
    unsigned int Parameter2: 2;/*Bit Mapping of the Second parameter in case of a jump type command*/
    unsigned int Parameter1: 3;/*Bit Mapping of the First parameter in case of a jump type command*/

} FirstWord;


/* definition for extra word for containig values of an Immediata or a label addres as defined in the instructions
 * manual- p15-17.*/
typedef struct {

    unsigned int ERA: 2;/*bits of the ERA field*/
    unsigned int Immidiate: 12; /*bits of the immediate value*/

} ImmediateWord;


/* definition for extra word for containing values of aregister (or two)  defined in the instructions
 * manual- p15-17.*/
typedef struct {

    unsigned int ERA: 2;/*bits of the ERA field*/
    unsigned int DestRegister: 6;/*bits of the Destination operand- it number*/
    unsigned int SourceRegister: 6;/*bits of the Source operand- it number*/

} RegisterWord;



/*A struct which is the actual word, can be any one of the above*/
typedef union
{
    FirstWord FIRST;
    ImmediateWord immediate;
    RegisterWord reg;
} word_union;



/*The instructions list node containing all the relevent data for the specific assembler word to be translater*/
typedef struct wnode
{
    int type; /*the word type contained  FirstWord,ImmediateWord or RegisterWord, enumerated via WORD_NUMBER in the
 *             header*/
    int address; /*The adress of the word as it will appear in the final .ob file*/
    word_union current_word; /*The word itself*/
    instructions_list_ptr next; /*a pointer to the next word*/
}Instruction_node;




instructions_list_ptr word_HEAD=NULL; /*The list's head*/
instructions_list_ptr word_TAIL=NULL; /*The list's tail*/


/*
 insetJumpCommand()
-----------------
A method for add a jump type command into the list.
It creates a node in the tree, stores the given values in it and adds the node to the list.

 The actual insertion is done woith the use of two functions, Set_Word() and   set_register_word() which receives
 appropriate flags and set the node values accordingly. These functions can be use for both jump type words and
 non jump typ words, and thus requires a flag signaling the type. more detail is provided in documentation of these
 functions.

 assumptions() valid input is given, meaning there are no errors in the flags and char combinations representing a line

 * */

void insetJumpCommand(int commandNum, char *DestOperand, int returnValueDest, int param1, int param1_flag, char *param1_char,
                      int param2, int param2_flag, char *param2_char, int LineNum, int IC)
        {
           /*node pointer which can be created depending on the values given */
            instructions_list_ptr First_Word = NULL, Second_Word = NULL, Third_Word = NULL, Forth_Word = NULL;

            /*creating the first word as defined in p15*/
            First_Word = InstuctionNodeAllocate(IC++); /*create the first instruction word structure as shown in p.15 of the instructions manuall*/
            First_Word->type=FIRST_WORD;


            First_Word->current_word.FIRST.ERA = ABSOLUTE_ENCODING; /*The first word is absolute*/
            First_Word->current_word.FIRST.DestAddressing = JUMP_ADDRESING;/*this is a jump command with jump addressing*/
            First_Word->current_word.FIRST.SourceAddressing = IMMEDIATE_ADDRESING;/*a jump command doesnt have asource
 *                                                                                 operand*/
            First_Word->current_word.FIRST.Opcode=(unsigned)commandNum;/*seting the opcode fiels of the word*/


            Second_Word = InstuctionNodeAllocate(IC++);/*at least one other word is created for the label*/

            Set_Word(NIL_ADDRESING, LABEL_JMP, Second_Word, returnValueDest, IC, DestOperand); /*a method for setting
 *                                             the values*/


            if(param1_flag==NIL_JMP)/*no other parameters are given*/
            {
                First_Word->current_word.FIRST.Parameter1 =0;
                First_Word->current_word.FIRST.Parameter2 =0;
            }
            else/*there are parameters*/
            {
                First_Word->current_word.FIRST.Parameter1 = (unsigned)param1_flag; /*seting the parameter jm encoing*/
                First_Word->current_word.FIRST.Parameter2 = (unsigned)param2_flag;

                Third_Word= InstuctionNodeAllocate(IC++); /*creating a word for the 1st parameter*/

                if(param1_flag==REGISTER_JMP && param2_flag==REGISTER_JMP) /*if bothe parameters are registers than only
 *                                                                        one word is needed*/
                    {
                        set_register_word(Third_Word,SOURCE_OPERAND,  param1); /*setting the values, SOURCE_OPERAND
 *                        is an enumeration of the operand type being set,defined in the header file*/
                        Third_Word->current_word.reg.DestRegister=(unsigned)param2; /* setting the second register*/
                    }
                else /*paramete 1 and parameter 2 are not both registers*/
                {
                    if (param1==REGISTER_JMP)/*check if parameter 1 is a register*/
                    {
                        set_register_word(Third_Word,DEST_OPERAND,  param1);
                        Third_Word->current_word.reg.SourceRegister=0; /*Parameter 2 was checked as not a register*/
                    }
                    else /*set the values for the parameter word which is not a register*/
                    {
                        Set_Word(NIL_ADDRESING, param1_flag,Third_Word, param1, IC, param1_char);
                    }

                    Forth_Word= InstuctionNodeAllocate(IC++);/*allocate space for the parameter 2 word*/

                    if (param2_flag==REGISTER_JMP)/*check if parameter 2 is a register*/
                    {
                        set_register_word(Forth_Word,DEST_OPERAND,  param2);
                        Forth_Word->current_word.reg.SourceRegister=0; /*Parameter 1 was checked as not a register*/
                    }
                    else/*set the values for the parameter word which is not a register*/
                    {
                        Set_Word(NIL_ADDRESING, param2_flag,Forth_Word,  param2, IC, param2_char);
                    }
                }
            }

            insertToWordsList(First_Word); /*insert the first created word into the instructions list*/

            insertToWordsList(Second_Word);/*insert the second created word into the instructions list*/

            if (Third_Word) /*if a third word was creaated, insert it*/
            {
                insertToWordsList(Third_Word);
            }

            if (Forth_Word) /*if a forth word was creaated, insert it*/
            {
                insertToWordsList(Forth_Word);
            }

        }

/*
 insertCommandToWordList()
-----------------
A method for add a non jump type command into the list. Similiar to the insetJumpCommand.
It creates a node in the tree, stores the given values in it and adds the node to the list.
It distinguishes between register type words and non register type words, which require different
 actions. This is because the register word has a diffrent format from the words of immediates and labels (the same
 word structure).

 The actual insertion is done woith the use of two functions, Set_Word() and  set_register_word() which receives
 appropriate flags and set the node values accordingly. These functions can be use for both jump type words and
 non jump typ words, and thus requires a flag signaling the type. more detail is provided in documentation of these
 functions.

 assumptions() valid input is given, meaning there are no errors in the flags and char combinations representing a line

 * */

void insertCommandToWordList(int command, char *SourceOperand, int SourceFlag, int returnValueSource, char *DestOperand,
                             int DestFlag, int returnValueDest, int LineNum, int IC) {
    instructions_list_ptr First_Word = NULL, Second_Word = NULL, Third_Word = NULL;



    First_Word = InstuctionNodeAllocate(IC++); /*create the first instruction word structure as shown in
 *                                                         p.15 of the instructions manuall*/

    First_Word->type=FIRST_WORD;

    First_Word->current_word.FIRST.ERA = ABSOLUTE_ENCODING;
    First_Word->current_word.FIRST.Opcode=(unsigned)command;

    First_Word->current_word.FIRST.Parameter1 = 0; /*this is not a jump type word thus the parameter fields are 0*/
    First_Word->current_word.FIRST.Parameter2 = 0;


    if (SourceFlag == NIL_ADDRESING) /*No Source Operand-*/
    {
        First_Word->current_word.FIRST.SourceAddressing = IMMEDIATE_ADDRESING;

        if (DestFlag == NIL_ADDRESING) /*No Destination Operand- no Operands type command*/
        {
            First_Word->current_word.FIRST.DestAddressing = IMMEDIATE_ADDRESING;
        }
        else /* One Operand type command- the second word belongs the the Destination Operand */
        {
            First_Word->current_word.FIRST.DestAddressing = (unsigned) DestFlag;


            Second_Word = InstuctionNodeAllocate(IC++); /*there is a dest operand- allocate space for it*/

            if (DestFlag==REGISTER_ADDRESING) /*if the operand is a register*/
            {
                set_register_word(Second_Word,DEST_OPERAND, DestFlag);
                Second_Word->current_word.reg.SourceRegister=0; /*no Source operand*/
            }
            else /*set the non register word*/
            {
                Set_Word(DestFlag, NIL_JMP,Second_Word, returnValueDest, IC, DestOperand);
            }
        }
    }

    else /*Their is a source Operand thus there must be a dest operand (one operands are only dest operands)*/
    {
        /*set the operand adressing in the first word*/
        First_Word->current_word.FIRST.SourceAddressing = (unsigned) SourceFlag;
        First_Word->current_word.FIRST.DestAddressing=(unsigned)DestFlag;

        Second_Word = InstuctionNodeAllocate(IC++);/*create a word for the source operand*/

        if (SourceFlag==REGISTER_ADDRESING && DestFlag== REGISTER_ADDRESING) /*if both operands are registre, only one
 *                                                       extra word is created */
        {
            set_register_word(Second_Word,SOURCE_OPERAND, returnValueSource); /*setting the source operand values*/
            Second_Word->current_word.reg.DestRegister=(unsigned)returnValueDest; /*setting the destination operand
 *                                                                                 values*/
        }

        else /*the operands are not both registers*/
        {
            if (SourceFlag==REGISTER_ADDRESING) /*check if the source operand is a register*/
            {
                set_register_word(Second_Word,SOURCE_OPERAND, returnValueSource);
                Second_Word->current_word.reg.DestRegister=0;
            }
            else /*the source operand is not a register*/
            {
                Set_Word(SourceFlag, NIL_JMP,Second_Word, returnValueSource, IC, SourceOperand);
            }


            Third_Word= InstuctionNodeAllocate(IC++); /*create a word for the destination operand*/


            if(DestFlag== REGISTER_ADDRESING) /*check if the destination operand is a register*/
            {
                set_register_word(Third_Word,DEST_OPERAND, returnValueDest);
                Third_Word->current_word.reg.SourceRegister=IMMEDIATE_ADDRESING;
            }
            else/*the destination operand is not a register*/
            {
                Set_Word(DestFlag, NIL_JMP,Third_Word, returnValueDest, IC, DestOperand);
            }


        }

    }

    /*insert the create words into the list*/
    insertToWordsList(First_Word);
    if (Second_Word)
    {
        insertToWordsList(Second_Word);
    }

    if (Third_Word)
    {
        insertToWordsList(Third_Word);
    }
}

/*
 InstuctionNodeAllocate()
-----------------
A method for allocating space for a node, return a pointer to the new node.
 * */

instructions_list_ptr InstuctionNodeAllocate(int address)
{
    instructions_list_ptr node;
    node = (instructions_list_ptr) malloc(sizeof(Instruction_node));/*allocate space for the node */
    if (!node) /* if allocation failed the pointer will be null*/
    {
        fprintf(stderr,"Error Creating a Wordnode!\n");
        exit(1);
    }

    /*set the node values to the given values*/
    node->address = address;
    node->next=NULL;
    return node;
}


/*
 insertToWordsList()
-----------------
A method for inserting a node to the list end, using the tail of the list.
 * */


void insertToWordsList(instructions_list_ptr node)
{
    if(word_HEAD==NULL) /*if the list is empty- the node becomes the head*/
    {
        word_HEAD=node;
        word_TAIL=node;
    }
    else /*the node is inserted at the tail*/
    {
        word_TAIL->next=node;
        word_TAIL=node;
    }
}



/*
 Set_Word()
-----------------
A method for setting the node values if the word is not of register type.
 The methods directly accesses the relevant fields in the node and sets the value passed down to it
 The method  distinguishes between jump and a normal command via the addresing recived.
 if the Command is a jump command, it is inputted with NIL_ADDRESING in the Command_Operand_Flag field and thus
 the Jump_Operand_flag is tested, other wise the Jump_Operand_flag in inputedd with the NIL_JUMP addressing
  and the Command_Operand_Flag addresing is tested.


  Also the External symbols table is created since this method is activated on word in the order in which they appear in
  the list, thus when an external symbol is encountered it is inserted into the external list, along with the word
  address.

  Assumption:
  Valid input is entered, e.g if Command_Operand_Flag!= NIL_ADRESSING than Jump_Operand_flag = NIL_JMP

 * */


void Set_Word(int Command_Operand_Flag,int Jump_Operand_flag, instructions_list_ptr Current, int Value, int IC, char *str)
{
    if (Command_Operand_Flag==IMMEDIATE_ADDRESING || Jump_Operand_flag==IMMEDIATE_JMP) /*set the fields for an
 *                                                                      imediate word*/
    {
        Current->type=IMMEDIATE_WORD;
        Current->current_word.FIRST.ERA = ABSOLUTE_ENCODING;
        Current->current_word.immediate.Immidiate=(unsigned)Value;

    }

    else if ( Command_Operand_Flag==DIRECT_ADDRESING || Jump_Operand_flag==LABEL_JMP) /*set the fields for a register
 *                                                                       word*/
    {
        symbol_ptr symbol; /*a pointer to search the symbol in the symbols table (again, assuming the table is complete)
 *                          */

        Current->type = IMMEDIATE_WORD;
        symbol = SearchSymbol(str); /*getting the symbol*/
        Current->current_word.immediate.Immidiate = (unsigned) SymbolAddress(symbol); /*setting the symbols adress,
 *                                                                                    which is correct at this stage*/

        if (isExternalSymbol(symbol) == TRUE) /*if the symbol in the list is external*/
        {
            Current->current_word.immediate.ERA = EXTERNAL_ENCODING;
            insertToExternList(str, IC - 1);/*The symbol is in the external list. IC was incremented and so IC-1
 *                                               is the correct address*/
        }
        else /*the symbol is not external*/
        {
            Current->current_word.immediate.ERA = RELOCATABLE_ENCODING;
        }
    }
}



/*
 * set_register_word()
-----------------
A method for setting the node values if the word is  of register type.
 The methods directly accesses the relevant fields in the node and sets the value passed down to it
 The method  distinguishes between Source Operands and Destination operand via  the Operand_TYPE
 enumeration.


  Assumptions:
  Valid input is entered.

 * */


void set_register_word( instructions_list_ptr Current,int OperandType, int Value)
{
    /*set the word values of the register word*/
    Current->type=REGISTER_WORD;
    Current->current_word.reg.ERA=IMMEDIATE_ADDRESING;

    if (OperandType==SOURCE_OPERAND)
    {
        Current->current_word.reg.SourceRegister=(unsigned)Value;/*The operand is a source operand*/
    }
    else
    {
        Current->current_word.reg.DestRegister=(unsigned)Value;/*The operand is a destination operand*/
    }
}









/*
 get_Word_Binary_Value()
-----------------
A method for returning the binary value of the word stored in the word pointer, used by the GenerateOutFiles() method
 in the FileFunction.c file.

 The word struct was defined using bit field and thought the insertion process this field were manipulated to represented
 the various assembly word code, e.g. for the first word the FIRST struct is 14 bits long with each bit defined as shown
 in the table at p15 of the instruction manual.

 Since the word nodes are user defined The method returns a pointer the reserved location for the word, which will
 contain the numerical value of the Word as it was set by the user.

 * */

unsigned short * get_Word_Binary_Value(instructions_list_ptr node)
{
    unsigned short * Word_binary_Code=0; /*The numeric value representing the word binary code*/
    if (node->type==IMMEDIATE_WORD ) /*if the word is an immediate, return the immediate word in the union field */
    {
        Word_binary_Code=(unsigned short *)(&(node->current_word.immediate));
    }
    else if(node->type==REGISTER_WORD)/*if the word is register, return the register word in the union field */
    {
        Word_binary_Code=(unsigned short *)(&(node->current_word.reg));
    }
    else if(node->type==FIRST_WORD)/*if the word is FirstWord (as in  the definition of p15 in the instructions manuall)
 *                                  , return the register word in the union field */
    {
        Word_binary_Code=(unsigned short *)(&(node->current_word.FIRST));
    }
    return Word_binary_Code;

}


/*
 returnInstructionsHead()
-----------------
A method for returning the list's head

 * */
void returnInstructionsHead(instructions_list_ptr *node)
{
    *node=word_HEAD;
}



/*
 getNextInstruction()
-----------------
A method for returning the next node of the given node (can be NULL)

 * */
instructions_list_ptr getNextInstruction(instructions_list_ptr node)
{
    return node->next;
}

/*
 getInstructionAddress()
-----------------
A method for returning the given nodes adreess

 * */

int getInstructionAddress(instructions_list_ptr node)
{
    return node->address;
}




/*
 freeInstructionslist()
-----------------
A method for freeing the list using a tmp pointer.
 The method iterates through the nodes using tmp to hold the current node and freeing it using the free()
 method
*/

void freeInstructionslist()
{
    instructions_list_ptr tmp;
    while (word_HEAD)
    {
        tmp=word_HEAD;
        word_HEAD=word_HEAD->next;
        free(tmp);
    }

}





