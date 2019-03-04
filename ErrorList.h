/*
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 *
 * ErrorList.h:
 * ----------------------------------------------------
 * This is the header file for the ErrorList structure.
 * This is a linked list storing details about errors and warnings detected during the translation of the .as file.
 * This header file contains declarations neccecary for the usage of the structure in other programs
 *
 *
 *
 * */

#ifndef PROJECT_ERRORLIST_H
#define PROJECT_ERRORLIST_H

#include "GlobalDeclarations.h"
/*enumerating the type of message that will be displayed to the user*/
typedef enum {ERROR,WARNING,NOTHING}errorType;
/*definition of a pointer to a node in the list*/
typedef struct errornode *error_ptr;

void insertToErrorList(char *Msg, int LineNum, errorType isError); /*insert Method*/
void printErrorList(char *fileName); /*Printing methods the prints all the nodes in the list*/

errorType hasError(); /*A methods that scans the list and return the Error type found within, if no nodes are present
 * than NOTHING is returned, else if there are any errors (even if there are Warnings) ERROR is returned*/

void freeErrorList(); /*a method to free the list*/


#endif 
