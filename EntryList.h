
/*
 *
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 *
 * EntryList.h:
 * ----------------------------------------------------
 * This is the header file for the EntryList data structure.
 * This is a linked list storing an a symbol defined in the assembly code as an entry, along with its
 *  address in the would be .ob file.
 *
 * This header file contains declarations necessary for the usage of the structure in other programs

 * */
#ifndef PROJECT_ENTRYLIST_H
#define PROJECT_ENTRYLIST_H

typedef struct en_node * entry_ptr;/*a definition of a pointer to a list node, the node contains:
 *                                    - The entry symbol
 *                                    - The address of the symbol in the file*/

void insertToEntryList(char *symbol, int address, int LineNumber); /*a method for creating and inserting  a node from
 *                                                                    given values into the Entrylist*/

void returnEntryHead(entry_ptr *node); /*a method for returning the head of the list*/

void UpdateEntryList(); /*a method for Updating the adresses of the entry nodes to their correct addresses once the
 *                        first pass is complete */

char* getEntrySymbol(entry_ptr node); /*a method for getting the symbol of the given node*/

int getEntryAddress(entry_ptr node); /*a method for getting the address of the given node*/

entry_ptr getNextEntry(entry_ptr node);/*a method for returning the next node to the given node*/

void freeEntrylist(); /*a method for freeing the list when it is not needed anymore*/
#endif 
