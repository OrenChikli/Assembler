/*
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 *
 FINAL PROJECT
===========

EntryList.c

Short Description:
------------------
 * A file defining the EntrysList data structure, which is a singly liked list for containing any .enty symbols
 * and their addresses.
 *
 * */



#include "EntryList.h"
#include "GlobalDeclarations.h"
#include "SymbolTable.h"
#include "ErrorList.h"

/*a definition of a list node*/
typedef struct en_node
{
    int address;/*The address of the symbol in the final .ob file*/
    char symbol[MAX_SYMBOL_SIZE]; /*The symbol*/
    entry_ptr  next;/*a pointer to the next node */
}Entry_node;


entry_ptr Entry_head=NULL;/*The list's head*/



/*
 EntryNodeAllocate()
-----------------
A method for allocating space for a node, return a pointer to the new node.
A temporary pointer to a node is allocated via the malloc() method, if the allocations succeeded,
the node values are set to the given values .
 * */

entry_ptr EntryNodeAllocate(char* symbol,int addr)
{
    entry_ptr node;
    node=(entry_ptr)malloc(sizeof(Entry_node));/*allocate space for the node */
    if(!node)/* if allocation failed the pointer will be null*/
    {
        fprintf(stderr,"Error Creating a Data node!\n");
        exit(1);
    }
    /*set the node values to the given values*/
    node->address=addr;
    strcpy(node->symbol,symbol);
    node->next=NULL;

    return node;

}

/*
insertToEntryList()
-----------------
A method for creating an entry node from given input, and inserting the node into the data list.
In the insertion the list is scanned, if the symbol exists in the list then this is an error and the node is not
 inserted.
 The method retuns a flag signaling the insertion success.
 * */
void insertToEntryList(char *symbol, int address, int LineNumber)
{
    bool StatusFlag=TRUE;
    entry_ptr current, tmp;
    current=EntryNodeAllocate(symbol,address); /*allocate space for the node*/

    if(Entry_head==NULL) /*The list is empty- the entry is the new head*/
    {
        current->next=NULL;
        Entry_head=current;
    }
    else/*scan the list*/
    {
        tmp=Entry_head;
        if (tmp->next==NULL && strcmp(tmp->symbol,symbol)==0 ) /*if there is only one node and it is has the same symbol
 *                                                               as the current node*/
        {
            StatusFlag=FALSE;
            insertToErrorList("Entry is already defined!",LineNumber,ERROR);
        }
        else /*There are more than one node in the list and the head symbol is not the same as the current one*/
        {
            while (StatusFlag==TRUE &&tmp->next!=NULL)/*scan the list as long as the symbol was not found in it or
 *                                                       the end of the list was reached*/
            {
                if (strcmp(tmp->symbol,symbol)==0)/*The symbol is found in the list*/
                {
                    StatusFlag=FALSE;
                    insertToErrorList("Entry is already defined!",LineNumber,ERROR);
                    free(current); /*The node is not connected to any other thus it needs to bee freed*/

                }
                tmp=tmp->next; /*move to the next node if all is ok*/
            }
        }
        if (StatusFlag==TRUE) /*the end of the list was reached- the symbol is not in the list*/
        {
            current->next=NULL;
            tmp->next=current;
        }
    }
}

/*
 returnDataHead()
-----------------
A method for returning the list's head

 * */


void returnEntryHead(entry_ptr *node)
{
    *node=Entry_head;
}


/*
 UpdateEntryList()
-----------------
A method for updating the Entry's address.
If the entry directive was encontered in the file before the definition of the symbol, it will get a non valid address.
 When this method is called, the symbols table is complete and so each node can have its address updated
 from the symbols table.
 The method iterates through the list and updates the symbol address.

 * */
void UpdateEntryList()
{
    symbol_ptr Symbol_tmp; /*a pointer to a symbol in the symbols table*/
    entry_ptr entry_tmp; /*a pointer to an entry in the entry list table*/
    entry_tmp=Entry_head;
    while (entry_tmp)/*scaning the list*/
    {
        Symbol_tmp=SearchSymbol(entry_tmp->symbol); /*find the symbol in the symbols list*/
        entry_tmp->address=SymbolAddress(Symbol_tmp); /*set the entry address to the address of the symbol*/
        entry_tmp=entry_tmp->next;
    }
}



/*
 getEntrySymbol()
-----------------
A method for returning the given node's stored symbol

 * */

char* getEntrySymbol(entry_ptr node)
{
    return node->symbol;
}


/*
 getEntryAddress()
-----------------
A method for returning the given nodes address

 * */

int getEntryAddress(entry_ptr node)
{
    return node->address;
}

/*
 getNextEntry()
-----------------
A method for returning the next node of the given node (can be NULL)

 * */

entry_ptr getNextEntry(entry_ptr node)
{
    return node->next;
}

/*
 freeEntrylist()
-----------------
A method for freeing the list using a tmp pointer.
 The method iterates through the nodes using tmp to hold the current node and freeing it using the free()
 method
*/

void freeEntrylist()
{
    entry_ptr tmp;
    while (Entry_head)
    {
        tmp=Entry_head;
        Entry_head=Entry_head->next;
        free(tmp);
    }

}

