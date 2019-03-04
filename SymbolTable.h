/*
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * SymbolTable.h:
 * ----------------------------------------------------
 * This is the header file for the SymbolTable.h data structure.
 *  This is a red black binary search tree acting as the symbols table for the file scan.
 *  it Stores:
 *        A given symbol,
 *        its address,
 *        A status flag indicating if it is an external symbol (isExternal),
 *        a status flag indicating if it is a complete label (isComplete,meaning its definition was found, not only
 *        the usage as described in the FirstPass() method in the FirstPass.c file) ,
 *        a status flag indicating if it is coupled to a command (isOperation)
 *        and the line number for its first appearance in the .as file  .
 *
 * This header file contains declarations necessary for the usage of the structure in other programs

 * */

#ifndef PROJECT_SYMBOLTABLE_H
#define PROJECT_SYMBOLTABLE_H

#include "GlobalDeclarations.h"



typedef struct node* symbol_ptr; /*definition of a pointer to a node in the tree*/


void insertToSymbolTable(char *symbol, int ADDRESS, int LineNumber, bool EXT, bool OP, bool complete);
/*a method for inserting a symbol and all its accompanying values into the tree */

bool isCompleteSymbol(symbol_ptr symbol_node); /*a method for getting the isComplete flag of the symbol */
bool isExternalSymbol(symbol_ptr symbol_node);/*a method for getting the isExternal flag of the symbol */
int SymbolAddress(symbol_ptr label_node);/*a method for getting the address of the symbol */
int SymbolLineNum(symbol_ptr label_node);/*a method for getting the line in which the symbol first apeared  */


symbol_ptr SearchSymbol(char*); /*a method for searching the symbol in the the tree*/
void UpdateSymbols(int);/*a method for updating the adsress of .data/.string associated symbols in the tree after the
 * FirstPass() method in the FirstPass.c file*/

void SetSymbolAdress(symbol_ptr Symbol, int add);/*a method for setting the address of the symbol to a given value */

void SetSymbolComplete(symbol_ptr Symbol, bool isComplete); /*a method for setting the isComplete flag of the symbol
 *                                                            to a given value */

void SetSymbolExt(symbol_ptr Symbol, bool isExt); /*a method for setting the isExternal flag of the symbol
 *                                                            to a given value */

void SetSymbolOP(symbol_ptr Symbol, bool isOP);  /*a method for setting the isOperation flag of the symbol
 *                                                            to a given value */



symbol_ptr find_missing(); /*a method for findind any incomplete sybols in the table*/

void FreeSymbolTable(); /*a method to free the list*/



#endif
