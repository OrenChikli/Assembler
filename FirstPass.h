
 /*
  *  FirstPass.h:
 * ----------------------------------------------------
 * This is the header file for the FirstPass.c file, it contain method declaration and definition used by ither methodes
  * to be used by other methods.
 *
 * The The first Pass functions concern the first pass stage in scanning the file
  *
  * */


#ifndef PROJECT_FIRSTPASS_H
#define PROJECT_FIRSTPASS_H

#include "GlobalDeclarations.h"


typedef enum {DEFINITION,USAGE}labelType; /*An enumeration to be used with the isLabel  Method:
 *                                         DEFINITION- the potential label was the first scanned word of the line,
 *                                         thus it is the definition of that label, e.g END: stop
 *                                         USAGE- Any other encounter with the potential label, meaning it is only
 *                                         used*/




void FirstPass(FILE *fp, int *IC, int *DC); /*A method which implements the first  pass algorithmic idea
 * presented in the course manual, explained in detail in the Assembler.c file*/

int isLabel(char *, int, labelType type); /*A method to analysed a given string and determine if it is a legal Label
 * acording to the assembler syntax*/

char *strtok_single(char *str, char const *delims);/*A user defined variation of the strtok method for scanning
 * a line up to a given delimiter.It performs the same task as strtok but with some differences.
 * more details re given in the Assembler.c file*/


 /*Enumerations for defining states with the removeSpaces function with remove spaces from string input*/
 enum REMOVE_SPACE_CASES {FIRST_REMOVE,NORMAL_REMOVE}; /*The case which is needed when the functions is called:
 * FIRST_REMOVE- removes only the spaces up to the first non space char
 * NORMAL_REMOVE-removes all the spaces found in the string.
 * */
 enum REMOVE_SPACE_RESULTS {EMPTY_SPACE,EXTRA_CONTENT,REMOVED_SPACES}; /*The possible cases the method encountered
 * EMPTY_SPACE- the string was found to be null or contained only whitespaces
 * EXTRA_CONTENT- a non space char was found after a string, meaning two "words" were found in the string
 * REMOVED_SPACES}- the space removal was successful*/


 int removeSpaces(char **str, int RemoveCase); /*A method for removing any spaces form a given string, whilst maintaining
 * user defined syntax rules . If any of the rules is violated  an error message is created and inserted to the error
 * mesages list*/
#endif 

