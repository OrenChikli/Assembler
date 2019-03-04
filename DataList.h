/*
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * DataList.h:
 * ----------------------------------------------------
 * This is the header file for the DataList data structure.
 *  This is a linked list storing an assembly data type word binary code representation at each node,
 * along with its address.
 * This header file contains declarations necessary for the usage of the structure in other programs

 * */


#ifndef PROJECT_DATALIST_H
#define PROJECT_DATALIST_H

typedef struct dnode * data_ptr;/*a definition of a pointer to a Data list node, the node contains:
 *                              - a 14 bit long numerical representation of a number that is either an immediate (for a
 *                                .data type directive) or the ascii representation of a char (for a .string type
 *                                directive).
 *                              - an address of the data word which is ultimatly its adress in the .ob file created
 *                              st the end of the file scan.*/

void insertToDataList(int number, int DC); /*a method for creating and inserting  anode from given values into the
 *                                           list*/

void returnDataHead(data_ptr *node); /*a method for returning the head of the list*/

void UpdateDataAddress(int IC); /*A method for updating the addresses of the data entries with the final IC*/

unsigned short getDataValue(data_ptr node); /*a method for geting the numerical representation of the data stored
 *                                             in the node*/

data_ptr getNextData(data_ptr node);/*a method for returning the next node to the given node*/


int getDataAddress(data_ptr node);/*a method for getting the address of the given node*/


void freeDataList(); /*a method for freeing the list when it is not needed anymore*/
#endif 

