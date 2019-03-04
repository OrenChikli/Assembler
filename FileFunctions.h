/*
 *
 *  * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * FileFunctions.h:
 * ----------------------------------------------------
 * This is the header file for the FileFunctions.c file, it contain method declaration to be used by other
 * files.
 *
 * The FileFunctions.c contains functions to manipulate files.
 *
 *
 *
 * */


#ifndef PROJECT_FUNCTIONSMAIN_H
#define PROJECT_FUNCTIONSMAIN_H
#include "GlobalDeclarations.h"


FILE *my_open(char *name, char *mode, int extension); /*a user defined version for opening a file given a file name
 * without its extention in the name, but as an enumerated value defined in the FILE_EXTENTIONS_ENUM herein.
 * The method preforms validity checks and open the full files (file name+ extention).*/

void GenerateOutFiles(int IC, int DC, char *name); /*a method for creating the output files at the end of a succsesful
 * scan of the given file*/


enum FILE_EXTENTIONS_ENUM {as_EXTENTION,ob_EXTENTION,ext_EXTENTION,ent_EXTENTION};
/*an enumeration of the various file extensions for use as flags to be transferred to file functions  */


#endif 
