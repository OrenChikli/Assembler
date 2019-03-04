/*
 *  * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 *  FirstPassHandlers.h:
 * ----------------------------------------------------
 * This is the header file for the FirstPassHandlers.c file, it contain method declaration to be used by other
 * files.
 *
 * The FirstPassHandlers.c contains functions to  analyse the line after an initial case was determined
 * The cases are: The line in a Directive or the line is a Command
 *
 * */


#ifndef PROJECT_COMMANDS_H
#define PROJECT_COMMANDS_H

#include "GlobalDeclarations.h"

enum COMMAND_TYPE {NO_OPERANDS,ONE_OPERAND,TWO_OPERANDS} ;
/*an enumeration of the Group the commands belong to
 *                 NO_OPERANDS- only the command is given with no etra content
 *                 ONE_OPERAND-the command has only one operand
 *                 TWO_OPERANDS-the command has Two operands*/


enum COMMAND_ENUM { MOVE_COMMAND, CMP_COMMAND, ADD_COMMAND, SUB_COMMAND, NOT_COMMAND,CLR_COMMAND,LEA_COMMAND,INC_COMMAND,
    DEC_COMMAND,JMP_COMMAND,BNE_COMMAND,RED_COMMAND,PRN_COMMAND, JSR_COMMAND,RTS_COMMAND,STOP_COMMAND} ;
/*An enumeration of the numerical codes of the commands corresponding the commands opcode as defined in p.15
 * of the cource instructions manual. e.g MOVE_COOMMAND corresponds to 0 for the mov command*/



enum DIRECTIVE_ENUM {DATA_DIRECTIVE,STRING_DIRECTIVE,ENTRY_DIRECTIVE,EXTERN_DIRECTIVE};
/*an enumeration of the various data directives for use as flags to be transferred to the DirectivesHandler()
 *  function*/


void DirectivesHandler(int Directive, int LineNumber, int *DC, bool hasLabel, int *IC); /*A method that checks
 * if the following line to th current scanned line (using strtok_single()) is a valid DIrective*/

void CommandHandler(int commandNumber, int LineNumber, int *IC);/*A method that checks
 * if the following line to th current scanned line (using strtok_single()) is a valid Command*/

#endif 
