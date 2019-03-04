/*
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * FirstPassList.h:
 * ----------------------------------------------------
 * This is the header file for the FirstPassList structure.
 * This is a linked list storing flag of scanned valid commands from the .as file
 * This header file contains declarations necessary for the usage of the structure in other programs

 * */

#ifndef PROJECT_FIRST_PASS_LIST_H
#define PROJECT_FIRST_PASS_LIST_H


typedef struct command_num* fp_list_ptr; /*a definition of a pointer to a list node*/


void  first_pass_jmp_insert(int commandNum, char *DestOperand, int returnValueDest, int param1, int param1_flag,
        char *param1_char, int param2, int param2_flag, char *param2_char, int LineNum, int IC);
/*a method for inserting a jump type command into the list*/


void first_pass_command_insert(int commandNumber, char *SourceOperand, int SourceFlag, int returnValueSource,
        char *DestOperand, int DestFlag, int returnValueDest, int LineNumber, int IC);
/*a method for inserting a  non jump type command into the list*/


void from_fplist_to_wordslist();
/*a method of  creating the InstructionsList defined in the InstructionsList.c file from the current FirstPassList.
 * The method is used by the secondPass method.*/

void insert_firstpass_to_instructions(fp_list_ptr node);
/*a method for inserting a FirstPassList node into the InstructionsList defined in the InstructionsList.c*/

void freeFirstPasslist(); /*a method for freeing the list after usage*/


#endif 
