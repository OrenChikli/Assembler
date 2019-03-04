/*
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * first_pass_list.h:
 * ----------------------------------------------------
 *  This  header file contains decelerations that are relevant to most functions in the project and thus is named as
 *  it was*/




#ifndef PROJECT_GLOBALDECLARATIONS_H
#define PROJECT_GLOBALDECLARATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*The initial Data Counter for the Data list*/
#define INITIAL_IC 100
#define INITIAL_DC 0

#define MAX_SYMBOL_SIZE 31 /*The maximal alowed size for a symbol/ label*/

#define MAX_LINE_SIZE 80 /*The maximum allwed size for  a line itn the scanned .as file*/



typedef enum {FALSE=-1,TRUE=1} bool; /*A definition of a equivalent to a boolean variable, with -1 as the false value*/


#endif 
