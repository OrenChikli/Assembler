
/*
 *
 *  * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * ExternList.h:
 * ----------------------------------------------------
 * This is the header file for the ExternList data structure.
 *  This is a linked list storing an a symbole defined in the assembly code as external, along with its
 *  address in the would be .ob file.
 *
 * This header file contains declarations necessary for the usage of the structure in other programs

 * */
#ifndef PROJECT_EXTERNLIST_H
#define PROJECT_EXTERNLIST_H


typedef struct ext_node * extern_ptr;/*a definition of a pointer to a list node, the node contains:
*                                    - The extern symbol
*                                    - The address of the symbol in the file*/

void insertToExternList(char* symbol, int addres);/*a method for creating and inserting a node from given values into
 *                                                the ExternList*/

void returnExternHead(extern_ptr *node); /*a method for returning the head of the list*/



char* getExternSymbol(extern_ptr node); /*a method for getting the symbol of the given node*/

int getExternAddress(extern_ptr node); /*a method for getting the address of the given node*/

extern_ptr getNextExtern(extern_ptr node);/*a method for returning the next node to the given node*/

void freeExternList(); /*a method for freeing the list when it is not needed anymore*/


#endif 
