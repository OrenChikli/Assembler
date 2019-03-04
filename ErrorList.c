/*
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 *
 FINAL PROJECT
===========

ErrorList.c

Short Description:
------------------
A file defining the ErrorList data structure, which is a singly liked list.
 The list contains error and warning messages that were discover during the file scannin.
 Each node contain the error mesage, it type (Error or a Warning) and the line number at which the error occured.

 Most of the errros are inserted in an asscending error, but some rechecks after the initiall scan can result in
  new errors, thus the list is maintained to have error in ascending line numer.

----------------------------------------
 */

#include "ErrorList.h"


char* ErrorType[2]={"ERROR:","WARNING:"}; /*An array to be used in storing the message in the list, the cell numbers
 * correspond to the ErrorType enumeration in the header file*/



 /*a definition of the error node */
struct errornode{
    errorType type; /*The type of error- WARNING or ERROR*/
    char errorMessage[MAX_LINE_SIZE]; /*The errror/Warning message*/
    int lineNumber; /*The line number in the file where the error was found*/
    error_ptr next; /*a pointer to the next node in the list */
};

error_ptr Error_Head=NULL; /*The list head*/


error_ptr ErrorNodeAllocate(); /*An internal method for allocating a node */


/*
 insertToErrorList()
-----------------
A method for inserting an error into the list, given the error message, its type and location in the file.

 The list is scanned from the head of the list and a node is inserted once it reaches the location where the next node
 has a greater line number, or the end of the list.
 * */


 void insertToErrorList(char *Msg, int LineNum, errorType isError)
{
    error_ptr tmp1,tmp2; /*temporary pointers for traversing the list*/
    tmp1=ErrorNodeAllocate(); /*first alocate a node*/

    /*set the node values*/
     tmp1->type=isError;
    strcpy(tmp1->errorMessage,Msg);
    tmp1->lineNumber=LineNum;
    tmp1->next=NULL;

    if(Error_Head==NULL) /*if the list is empty, the node becomes the head*/
    {
        Error_Head=tmp1;
    }
    else/*the list is not empty, move the error to its positin determined by ascending line number*/
    {
        tmp2=Error_Head;/*set the pointer to begine traversal*/
        if (tmp2->lineNumber>=LineNum) /*if the head's line number is smaller than the current node's numer-
 * the current node is to be the head*/
        {
            tmp1->next=tmp2;
            Error_Head=tmp1;
        }
        else /*the node needs to be inserted to the list*/
            {
            if (tmp2->next && tmp2->lineNumber < LineNum) /*iterate as long there are nodes in the list and the current
 *                                                       one's line number is greater then the given node*/
            {
                while (tmp2->next && tmp2->next->lineNumber < LineNum) {
                    tmp2 = tmp2->next;
                }
            }

            if (tmp2->next) /*if the scan was terminated becuase the line number of the current node is greater than the
 * number of the given node*/
            {
                tmp1->next = tmp2->next;
                tmp2->next = tmp1;
            }
            else /*the node is the last in the list*/
                {
                tmp2->next = tmp1;
                }
        }
    }
}


/*
 ErrorNodeAllocate()
-----------------
A methode for  allocating space for a node.
 * */
error_ptr ErrorNodeAllocate()
{
    error_ptr tmp;
    tmp =(error_ptr)malloc(sizeof(struct errornode));
    if(!tmp)
    {
        printf("No room in the error list, terminating.\n");
        exit(1);
    }
    return tmp;
}

/*
 printErrorList()
-----------------
A method for printing the errors list, given the file name.
 the method scans the list from the head up to end at prints out the error message type,
 the message it self, and the line in which it was detected
 * */

void printErrorList(char *fileName)
{
    error_ptr tmp;
    tmp=Error_Head;
    if (tmp)
    {

        fprintf(stderr,"Error List for file %s.as:\n\n",fileName); /*P rint the message for the given file name*/
        while(tmp)
        {
           /*accses the node typ, message and line number and print them*/
            fprintf(stderr,"%s %s, at line %d\n",ErrorType[tmp->type],tmp->errorMessage,tmp->lineNumber);
            tmp=tmp->next;
        }
        printf("\n");
    }


}

/*
 ErrorNodeAllocate()
-----------------
A method for checking if the errors list conatins any errors.
 Returns the type of error found, if an Error was found it will return ERROR, regardles to if the list has warnings
 * */
errorType hasError()
{
     errorType flag=NOTHING;
     error_ptr tmp;
     tmp=Error_Head;
    while(tmp)
    {
        if (flag!=ERROR)
        {
            flag=tmp->type;
        }

        tmp=tmp->next;
    }
    return flag;

}


/*
freeErrorList()
-----------------
A method for freeing the list using a tmp pointer.
 The method iterates through the nodes using tmp to hold the current node and freeing it using the free()
 method
 * */


void freeErrorList()
{
    error_ptr tmp;
    while (Error_Head)
    {
        tmp=Error_Head;
        Error_Head=Error_Head->next;
        free(tmp);
    }

}
