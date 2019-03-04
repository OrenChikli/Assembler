/*
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * FINAL PROJECT
 * ===========
 * ExternList.c
 *
 * Short Description:
 * ------------------
 *
 *
 *
 * A file defining the ExternList data structure, which is a singly liked list for containing any .extern symbols
 * and their addresses.
 *
 * The insertion to this list is done in a non changing order, meaning each nodes location is final and
 * thus a Tail node is created for better time complexity insertion at the end of the list
 * */



#include "ExternList.h"
#include "SymbolTable.h"
#include "GlobalDeclarations.h"


/*a definition of a list node*/
typedef struct ext_node
{
    int address;/*The address of the symbol in the final .ob file*/
    char symbol[MAX_SYMBOL_SIZE];/*The symbol*/
    extern_ptr  next;/*a pointer to the next node */
}Eextern_node;


extern_ptr Extern_HEAD=NULL;/*The list's head*/
extern_ptr Extern_TAIL=NULL; /*The list's tail*/

/*
 ExternNodeAllocatee()
-----------------
A method for allocating space for a node, return a pointer to the new node.
A temporary pointer to a node is allocated via the malloc() method, if the allocations succeeded,
the node values are set to the given values .
 * */

extern_ptr ExternNodeAllocate(char* symbol,int addr)
{
    extern_ptr node;
    node=(extern_ptr)malloc(sizeof(Eextern_node));/*allocate space for the node */
    if(!node)/* if allocation failed the pointer will be null*/
    {
        fprintf(stderr,"Error Creating a Extern node!\n");
        exit(1);
    }
    /*set the node values to the given values*/
    node->address=addr;
    strcpy(node->symbol,symbol);
    node->next=NULL;
    return node;

}
/*
insertToExternList()
-----------------
A method for creating a Data node from given input, and inserting the node into the data list.
 * */

void insertToExternList(char* symbol, int addres)
{
    extern_ptr current;
    current=ExternNodeAllocate(symbol,addres);/*allocate space for the node */

    if(Extern_HEAD==NULL)/*if the list is empty- the node becomes the head*/
    {
        Extern_HEAD=current;
        Extern_TAIL=current;
    }
    else/*the node is inserted at the tail*/
    {
        Extern_TAIL->next=current;
        Extern_TAIL=current;
    }
}

/*
returnExternHead()
-----------------
A method for returning the list's head

 * */

void returnExternHead(extern_ptr *node)
{
    *node=Extern_HEAD;
}

/*
 getExternSymbol()
-----------------
A method for returning the given node's stored symbol

 * */
char* getExternSymbol(extern_ptr node)
{
    return node->symbol;
}


/*
 getExternAddress()
-----------------
A method for returning the given nodes address

 * */
int getExternAddress(extern_ptr node)
{
    return node->address;
}

/*
getNextExtern()
-----------------
A method for returning the next node of the given node (can be NULL)

 * */
extern_ptr getNextExtern(extern_ptr node)
{
    return node->next;
}

/*
 freeExternList()
-----------------
A method for freeing the list using a tmp pointer.
 The method iterates through the nodes using tmp to hold the current node and freeing it using the free()
 method
*/
void freeExternList()
{
    extern_ptr tmp;
    while (Extern_HEAD)
    {
        tmp=Extern_HEAD;
        Extern_HEAD=Extern_HEAD->next;
        free(tmp);
    }

}
