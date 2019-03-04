/*
 *
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 *
 FINAL PROJECT
===========

FirstPassList.c

Short Description:
------------------
A file defining the FirstPassList data structure, which is a singly liked list for containing flags of commands after
 the first pass on the list. IF the first pass does not produce any errors, this list is read during the second pass
 and traslated into the instructions list (InstructionsList.c) which contains the words of the assembly code in a way
 that can later be printed in the needed binary representation.

A node is a union between 2 structs- 1 for a normal command, and the other for a jump type command

  * The insertion to this list is doen in a non changing order, meaning each nodes location is final and
 * thus a Tail node is created for better time coplexity insertion at the end of the list

----------------------------------------
 */
#include "FirstPassList.h"
#include "GlobalDeclarations.h"
#include "InstructionsList.h"

 /*a node of a normal command flags and operands*/
typedef struct
{
    int commandNumber; /*The command number as defined bf the commands enum in COMMAND_ENUM at the FirstPass.h
 *                       the list does not deal with the enumerations directly, but only numerical values*/
    char DestOperand[MAX_SYMBOL_SIZE]; /*a string to hold the Destination Operand if the Destination operand is a label*/
    int returnValueDest; /*The numerical value of the destination Operand if not a label, a register number or an immediate*/
    int DestFlag; /*A flag signaling the Destinations operand's addressing method*/
    char SourceOperand[MAX_SYMBOL_SIZE];/*a string to hold the Destination Operand if the Destination operand is a label*/
    int SourceFlag;/*A flag signaling the Source operand's addressing method*/
    int returnValueSource;/*The numerical value of the source Operand if not a label, a register number or an immediate*/
    int LineNumber; /*The line number in the .as file from which the command was read*/
    int IC; /*The instruction counter fot the word*/
}Command_flags;

/*a node of a jump type commands flags and operands*/
typedef struct
{
    int commandNumber;/*The command number as defined bf the commands enum in COMMAND_ENUM at the FirstPass.h
 *                       the list does not deal with the enumerations directly, but only numerical values*/
    char DestOperand[MAX_SYMBOL_SIZE];/*a string to hold the Destination Operand if the Destination operand is a label*/
    int returnValueDest; /*a flag signaling wheter the symbol is complete*/
    int param1; /*if parameter 1 is not a label this flag holds it numerical value, a register or an immediate*/
    int param1_flag; /*the jump encoding of the parameter, as enumerated in the JUMP_PARAMETER_ADDRESSING in the p
 *                      InstructionsList.h file*/
    char param1_char[MAX_SYMBOL_SIZE];/*a string to hold the label for parameter1 if it is a label */
    int param2;/*if parameter 2 is not a label this flag holds it numerical value, a register or an immediate*/
    int param2_flag;/*the jump encoding of the parameter, as enumerated in the JUMP_PARAMETER_ADDRESSING in the p
 *                      InstructionsList.h file*/
    char param2_char[MAX_SYMBOL_SIZE];/*a string to hold the label for parameter2 if it is a label */
    int LineNumber; /*The line number in the .as file from which the command was read*/
    int IC; /*The instruction counter fot the word*/
}Jmp_Flags_node;


/* a union representation an actul command in the list*/
typedef union
{
    Jmp_Flags_node jump_command;
    Command_flags command;
} command_list_node;



/*a struct defining  a node in the first_pass list*/
typedef struct command_num
{
    bool isJump; /*a flag for the type of node, a jump type command or a regular command*/
    command_list_node current; /*the command it self*/
    fp_list_ptr next; /*a pointer to the next node in the list*/
}command_num_node;


fp_list_ptr first_pass_HEAD=NULL; /*the list head*/
fp_list_ptr first_pass_TAIL=NULL; /*The list tail*/


/*
 First_pass_NodeAllocate()
-----------------
A method for  allocating space for a node, return a pointer to the new node.
 * */

fp_list_ptr First_pass_NodeAllocate()
{
    fp_list_ptr node;
    node=(fp_list_ptr)malloc(sizeof(command_num_node));
    if(!node)
    {
        printf("Error Creating a Data node!\n");
        exit(1);
    }
    node->next=NULL;
    return node;

}
/*
 first_pass_node_insert()
-----------------
A method for inserting a given node via a pointer into the list at its tail.*/

void first_pass_node_insert(fp_list_ptr node)
{

    if(first_pass_HEAD==NULL) /*if the list is empty- the node becomes the head*/
    {
        first_pass_HEAD=node;
        first_pass_TAIL=node;
    }
    else /*the node is inserted at the tail*/
    {
        first_pass_TAIL->next=node;
        first_pass_TAIL=node;
    }
}



/*
first_pass_jmp_insert()
-----------------
A method for inserting the given flags and strings of a scanned jump type command into the list via the
 first_pass_node_insert method
 * */

void  first_pass_jmp_insert(int commandNum, char *DestOperand, int returnValueDest, int param1, int param1_flag, char *param1_char,
                           int param2, int param2_flag, char *param2_char, int LineNum, int IC)
{
    fp_list_ptr node;
    node=First_pass_NodeAllocate(); /*create a node*/

    node->isJump=TRUE; /*The node is of jump type command*/


     /*setting the node values to the given values*/
    node->current.jump_command.commandNumber=commandNum;
    strcpy(node->current.jump_command.DestOperand,DestOperand);
    node->current.jump_command.returnValueDest=returnValueDest;
    node->current.jump_command.param1=param1;
    node->current.jump_command.param1_flag=param1_flag;

    node->current.jump_command.param2=param2;
    node->current.jump_command.param2_flag=param2_flag;

    node->current.jump_command.LineNumber=LineNum;
    node->current.jump_command.IC=IC;


    /* if the given operand is a label, then the parm_char string is valid and needs to be copied into the specific
     * param char*/
    if (param1_flag==LABEL_JMP)
    {
        strcpy(node->current.jump_command.param1_char,param1_char);
    }


    if (param2_flag==LABEL_JMP)
    {
        strcpy(node->current.jump_command.param2_char,param2_char);
    }

    first_pass_node_insert(node); /*insert the node into the list */
}

/*
first_pass_jmp_insert()
-----------------
A method for inserting the given flags and strings of a scanned non jump type command into the list via the
 first_pass_node_insert method*/

void first_pass_command_insert(int commandNumber, char *SourceOperand, int SourceFlag, int returnValueSource,
                               char *DestOperand, int DestFlag, int returnValueDest, int LineNumber, int IC)
{
    fp_list_ptr node;
    node=First_pass_NodeAllocate();/*create a node*/

    node->isJump=FALSE; /*The node is not a jump type command*/

    node->current.command.commandNumber=commandNumber; /* set the command number*/

     /*setting the Source operand flags*/
    node->current.command.SourceFlag=SourceFlag;
    node->current.command.returnValueSource=returnValueSource;

    if (SourceFlag==DIRECT_ADDRESING)
    {
        strcpy(node->current.command.SourceOperand,SourceOperand);
    }

    /*setting the Destination operand flags*/
    node->current.command.DestFlag=DestFlag;
    node->current.command.returnValueDest=returnValueDest;

    if (DestFlag==DIRECT_ADDRESING)
    {
        strcpy(node->current.command.DestOperand,DestOperand);
    }

    /*setting the rest of the flags*/
    node->current.command.LineNumber=LineNumber;
    node->current.command.IC=IC;

    first_pass_node_insert(node); /*inserting the node to the list*/
}


/*
insert_firstpass_to_instructions()
-----------------
A method used by the second pass method for inserting a node into the InstructionsList, which in turn create the actuall
 assembly wrod represntations to be later printed out
*/

void insert_firstpass_to_instructions(fp_list_ptr node)
{

    if (node->isJump==TRUE)/*use the values of the jump command*/
    {
        /*for clarity purposes, all the node value are pass into parameters which are then passed into the
         * insetJumpCommand  */
        int commandNum=node->current.jump_command.commandNumber;
        char* DestOperand=node->current.jump_command.DestOperand;
        int returnValueDest=node->current.jump_command.returnValueDest;

        int param1=node->current.jump_command.param1;
        int param1_flag=node->current.jump_command.param1_flag;
        char *param1_char=node->current.jump_command.param1_char;

        int param2=node->current.jump_command.param2;
        int param2_flag=node->current.jump_command.param2_flag;
        char *param2_char=node->current.jump_command.param2_char;

        int LineNum=node->current.jump_command.LineNumber;
        int IC=node->current.jump_command.IC;

        insetJumpCommand(commandNum, DestOperand,returnValueDest,param1, param1_flag, param1_char,
        param2, param2_flag, param2_char, LineNum, IC);
    }
    else/*use the values of the non jump command*/
    {
        /*for clarity purposes, all the node values are passed into parameters which are then passed into the
        * insertCommandToWordList  */

        int commandNumber=node->current.command.commandNumber;

        char* SourceOperand=node->current.command.SourceOperand;
        int SourceFlag=node->current.command.SourceFlag;
        int returnValueSource=node->current.command.returnValueSource;

        char* DestOperand=node->current.command.DestOperand;
        int DestFlag=node->current.command.DestFlag;
        int returnValueDest=node->current.command. returnValueDest;

        int LineNumber=node->current.command.LineNumber;
        int IC=node->current.command.IC;

        insertCommandToWordList(commandNumber, SourceOperand, SourceFlag, returnValueSource, DestOperand, DestFlag,
                                returnValueDest, LineNumber, IC);
    }
}


/*
from_fplist_to_wordslist()
-----------------
A method used by the second pass method for inserting the FirstPassList into the Instructions list, each node at a
 time
*/

void from_fplist_to_wordslist()
{
    fp_list_ptr first_pass_tmp; /*a temporary pointer to run throught the list*/
    first_pass_tmp=first_pass_HEAD; /*set it to head and continue from it*/
    while(first_pass_tmp)
    {
        insert_firstpass_to_instructions(first_pass_tmp);
        first_pass_tmp=first_pass_tmp->next;
    }
}



/*
freeFirstPasslist()
-----------------
A method for freeing the list using a tmp pointer.
 The method iterates through the nodes using tmp to hold the current node and freeing it using the free()
 method
*/
void freeFirstPasslist()
{
    fp_list_ptr tmp;
    while (first_pass_HEAD)
    {
        tmp=first_pass_HEAD;
        first_pass_HEAD=first_pass_HEAD->next;
        free(tmp);
    }

}
