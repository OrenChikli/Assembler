
/*
 *
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 *
 FINAL PROJECT
===========

DataList.c

Short Description:
------------------
 * A file defining the DataList data structure, which is a singly liked list for containing a representation
 * of a data word  (similar to the instructions list that hold instruction words)  in assembly syntax,
 * derived form a .data/.string directive in the .as file
 *
 * The data list is a list of all the data words scanned- it contain a 14 bit representation of the word and its
 * address.
 * The address is initially in relation to the insertion oreder against the INITIAL_IC,
 * e.g for an INITIAL_IC of 0, the 5'th data word will have and initial address of 5. This address is updated at the
 * end of the firstPass() method (defined in the firstPass.c file) with the final DC, to correspond with the appending
 * of the data words to the end of the instruction words.
 *
 *
 * The insertion to this list is done in a non changing order, meaning each nodes location is final and
 * thus a Tail node is created for better time complexity insertion at the end of the list
 *

 *
 * */

#include "DataList.h"
#include "GlobalDeclarations.h"
#include "ErrorList.h"

/*a definition of a list node*/
typedef struct dnode
{
    int address; /*The address of the data word, first given the DC value, and updated at the end with the addition of
 *                the DC value/*/
    unsigned int value:14; /*the value of the data entry, 14 bit limit of the word size*/
    data_ptr  next; /*s pointer to the next node*/
}Data_node;


data_ptr data_HEAD=NULL; /*The list's head*/
data_ptr data_TAIL=NULL;/*The list's tail*/

/*
 DataNodeAllocate()
-----------------
A method for allocating space for a node, return a pointer to the new node.
A temporary pointer to a node is allocated via the malloc() method, if the allocations succeeded,
the node values are set to the given values .
 * */

data_ptr DataNodeAllocate(int num,int addr)
{
    data_ptr node;
    node=(data_ptr)malloc(sizeof(Data_node));/*allocate space for the node */
    if(!node)/* if allocation failed the pointer will be null*/
    {
        fprintf(stderr,"Error Creating a Data node!\n");
        exit(1);
    }
    /*set the node values to the given values*/
    node->address=addr;
    node->value=(unsigned)num;
    node->next=NULL;

    return node;

}


/*
insertToDataList()
-----------------
A method for creating a Data node from given input, and inserting the node into the data list.
 * */


void insertToDataList(int number, int DC)
{
    data_ptr node;
    node=DataNodeAllocate(number,DC);/*allocate space for the node */


    if(data_HEAD==NULL) /*if the list is empty- the node becomes the head*/
    {
        data_HEAD=node;
        data_TAIL=node;
    }
    else /*the node is inserted at the tail*/
    {
        data_TAIL->next=node;
        data_TAIL=node;
    }

}

/*
 returnDataHead()
-----------------
A method for returning the list's head

 * */

void returnDataHead(data_ptr *node)
{
    *node=data_HEAD;
}



/*
 UpdateDataAddress()
-----------------
A method for updating the addresses of all the data nodes with the given IC value.
 This method is called at the end of the firstPass method, and IC is the end of the instructions counter, thus
 the address of each node is DC+IC, which corresponds to the connection of the data list after the instructions when
 creating the outpout .ob file with the GenerateOutFiles() metod in the FileFunctions.c file.

 The methods begins at the list head and simply adds IC to each node iteritavly until the end is reached

 * */

void UpdateDataAddress(int IC)
{
    data_ptr node;
    node=data_HEAD;
    while (node)
    {
        node->address += IC;
        node=node->next;

    }

}


/*
 getDataValue()
-----------------
A method for returning the given node's stored numerical value (can be the ascii code of a char)

 * */

unsigned short getDataValue(data_ptr node)
{
    return (unsigned short)node->value;
}

/*
 getNextData()
-----------------
A method for returning the next node of the given node (can be NULL)

 * */

data_ptr getNextData(data_ptr node)
{
    return node->next;
}


/*
 getDataAddress()
-----------------
A method for returning the given nodes address

 * */
int getDataAddress(data_ptr node)
{
    return node->address;
}



/*
 freeDataList()
-----------------
A method for freeing the list using a tmp pointer.
 The method iterates through the nodes using tmp to hold the current node and freeing it using the free()
 method
*/

void freeDataList()
{
    data_ptr tmp;
    while (data_HEAD)
    {
        tmp=data_HEAD;
        data_HEAD=data_HEAD->next;
        free(tmp);
    }

}
