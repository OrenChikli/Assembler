/*
 *
 *  * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 FINAL PROJECT
===========

FirstPassHandlers.c

Short Description:
------------------
A file containing several function for the handler functions that analysed a given line to determine if it maches any
 of the criteria for.

Assumptions:
FirstPass preformed correct analysis so far.





----------------------------------------

 */
#include "FirstPassHandlers.h"
#include "GlobalDeclarations.h"
#include "FirstPass.h" /*Include for the usage of strtok_single and removespaces*/


#include "ErrorList.h"
#include "SymbolTable.h"
#include "InstructionsList.h"
#include "DataList.h"
#include "EntryList.h"
#include "ExternList.h"
#include "FirstPassList.h"


#define MAX_IMMEDIATE_VALUE (4096) /*The maximal allowed value  for a 12 bit Immediate in 2's compliment*/
#define MIN_IMMEDIATE_VALUE (-4096)/*The minimal allowed value  for a 12 bit Immediate in 2's compliment*/

#define NUM_OF_REGISTERS 8



/*An array of strings containing a register string, where each cell number corresponds to the register number */
static char* REGISTERS[NUM_OF_REGISTERS]={"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};



#define BASE 10 /*the base by which the strol method in the isImmediate function create a number form a string*/


int isRegister(char * str); /*An internal method for checking if the current string is a register*/
int isImmediate(char *str, int LineNumber);/*An internal method for checking if the current string is a number*/
bool hasSpaces(char * str); /*An internal method for checking if the current string as any spaces*/
int getCommaNum(char* str);/*An internal method for counting the total number of commas in a given string*/



/*
 DirectivesHandler()
-----------------
a method for determining if the given line (via the use of strtok_single()) is a valid Directive.
 the method is given the Directive enumeration, the current line number, a pointer to the Datacounter, a flag
 for whether the FirstPass method detected a label before the directive and a pointer th the Instructions Counter
 * */



void DirectivesHandler(int Directive, int LineNumber, int *DC, bool hasLabel, int *IC)
{

    char *current; /*The current analysed str*/
    bool StatusFlag=TRUE; /*The Status of the current check*/
    int Remove_flag; /*the reult of remove spaces flag*/





    if (Directive == DATA_DIRECTIVE) /*first pass identified a Data directive*/
    {
        int numOfCommas; /*an integer fo the number of command in the remaining data input string*/
        int num_of_data_words=0; /*the number of words that will be scanned succsesfuly, incremented at each scan*/
        int tmp_num; /*a tempory integer to get the isImediate result*/

        if(hasLabel==FALSE) /*if the data directive does not have a label before it*/
        {
            insertToErrorList("Data directive without a label", LineNumber, WARNING);
        }

        current=strtok_single(NULL,"\n"); /*getting the first data entry from the input*/
        if (removeSpaces(&current, FIRST_REMOVE)==EMPTY_SPACE) /*remove the space up yo the first number/char*/
        {
            insertToErrorList("Directive with no input", LineNumber, ERROR);

        }



        else /*the row is not empty */
        {
            numOfCommas=getCommaNum(current); /*count the number of commas in the string*/
            current=strtok_single(current,","); /*scan for the next number up to a comma*/
            if((Remove_flag=removeSpaces(&current, NORMAL_REMOVE)) != REMOVED_SPACES) /*remove the spaces in the
 *                                                                        scanned str*/
            {

                if(Remove_flag==EMPTY_SPACE) /*meaning a comma was found before the first number*/
                {
                    insertToErrorList("Comma detected before first number in .data directive", LineNumber, ERROR);
                }
                else /* two space separated words were found*/
                {
                    insertToErrorList("No comma after number in .data directive", LineNumber, ERROR);
                }

            }
            else /*spaces were successfully removed*/
            {
                while (StatusFlag != FALSE && current) /*scan the entire line*/
                {
                    tmp_num = isImmediate(current, LineNumber); /*check if the scanned number is valid*/


                    if (tmp_num < MAX_IMMEDIATE_VALUE) /*The number is valid*/
                    {
                        if (current[0]=='#') /*if it is a valid number, i cannot begin with a #, the method isImmediate
 *                                           ignores this symbol*/
                        {
                            insertToErrorList("numbers of data directive are written without the # sign", LineNumber, ERROR);
                            StatusFlag = FALSE;
                        }
                        else /*a valid number is given for the directive*/
                        {
                            insertToDataList(tmp_num, (*DC)++); /*insert the number in the data list*/
                            num_of_data_words++;/*increment the data words count*/
                            current = strtok_single(NULL, ","); /* get the next data number*/
                            if (current) /* if currents inst NULL*/
                            {
                                if ((Remove_flag=removeSpaces(&current, NORMAL_REMOVE))!=REMOVED_SPACES) /*check for
 *                                                                                             correct space removal*/
                                {
                                    StatusFlag=FALSE;
                                    if (Remove_flag==EMPTY_SPACE)
                                    {
                                        insertToErrorList("Double comma (',') detected in .data directive", LineNumber, ERROR);
                                    }
                                    else
                                    {
                                        insertToErrorList("No comma after number in .data directive", LineNumber, ERROR);
                                    }
                                }
                            }
                        }

                    }
                    else /*The number is not valid*/
                    {
                        StatusFlag = FALSE;
                        insertToErrorList("Not a valid number detected at .data directive", LineNumber, ERROR);
                    }
                }

                if (StatusFlag==TRUE && numOfCommas>=num_of_data_words) /*if all the line was scanned successfully,
 *                   the strtok_single cannot tell if the string ended with a comma or not, thus this tests for
 *                   a smaller  number of commas  than the number of words word */
                {
                    insertToErrorList("Comma detected after the last data number", LineNumber, ERROR);
                }
            }
        }



    }

    else if (Directive == STRING_DIRECTIVE) /*first pass identified a String directive*/
    {
        if(hasLabel==FALSE)/*if the string directive does not have a label before it*/
        {
            insertToErrorList("String directive without a label", LineNumber, WARNING);
        }


        current=strtok_single(NULL,"\n");/*get the whole line*/

        if(removeSpaces(&current, FIRST_REMOVE) !=REMOVED_SPACES) /*remove spaces from the beginning*/
        {
            insertToErrorList(".string operand with no given string", LineNumber, ERROR);

        }

        else /*the string is not empty*/
        {
            int i=0; /*a counter for iterating through the string*/
            if (current[i]!='\"')/*the first char of the string  should be  double quotes*/
            {
                insertToErrorList("A string in the string directive needs to begin with double quoets", LineNumber, ERROR);
            }
            else /*the first char is double quotes as needed*/
            {
                i++;/*move to the next char in the string*/
                while (current[i]!='\0' && current[i]!='\"') /*scan the string as long as the end of the string is not
 *                                                   reached or the terminating double quotes are found*/

                {
                    insertToDataList(current[i],(*DC)++);/*insert the current character into the data list*/
                    i++;
                }

                if (current[i]!='\"') /*if the string does not have the closing double quotes*/
                {
                    insertToErrorList("A string in the string directive needs to end with double quoets", LineNumber, ERROR);

                }
                else /*a Correct string was scanned*/
                {
                    i++; /*The rest of string can be null or only spaces*/
                    while(current[i]!='\0' && StatusFlag==TRUE) /*continue to scan the string, only spaces are allowed
 *                                                          from now on*/
                    {
                        if(isspace(current[i])) /*while the scanned char is a space*/
                        {
                            i++;
                        }
                        else /*extra content was found*/
                        {
                            insertToErrorList("Extrenaous content after .string directive", LineNumber, ERROR);
                            StatusFlag=FALSE;
                        }
                    }


                    if (StatusFlag==TRUE)/*if the rest was only spaces or the end of the string was reached,
 *                                         a valid string was given*/
                    {
                        insertToDataList('\0',(*DC)++);
                    }
                }


            }

        }

    }
    else if (Directive == ENTRY_DIRECTIVE) /*first pass identified an Entry directive*/
    {
        symbol_ptr tmpSymbol; /*a pointer for interacting with the symbol Table*/
        current=strtok_single(NULL,"\n"); /*get the remaining line*/

        if (hasLabel==TRUE) /*A label definition before an entry is meaningless*/
        {
            insertToErrorList("Label definition before  .enty directive is meaningless", LineNumber, WARNING);
        }

        if ((Remove_flag= removeSpaces(&current, NORMAL_REMOVE)) != REMOVED_SPACES) /*remove spaces from the string*/
        {
            if (Remove_flag == EXTRA_CONTENT) /* more than one word detected*/
            {
                insertToErrorList("Extrenaous content after  label", LineNumber, ERROR);
            }
            else /*empty string*/
            {
                insertToErrorList("No Label detected for .entry Directive", LineNumber, ERROR);
            }
        }

        else /*non empty  one word string was given*/
        {

            tmpSymbol = SearchSymbol(current); /*search the symbols table for the scanned string*/
            if (!tmpSymbol) /*insert the label to the symbols table if not there*/
            {
                insertToSymbolTable(current, *IC, LineNumber, FALSE, FALSE, FALSE); /*This is a an internal label
 *                                                        signaling an operation and so this label is complete*/
            }

            insertToEntryList(current, *IC, LineNumber);/*insert to entry list*/
        }
    }

    else if (Directive == EXTERN_DIRECTIVE) /*first pass identified a Data directive*/
    {
        symbol_ptr tmpSymbol; /*a pointer for interacting with the symbol Table*/
        current=strtok_single(NULL,"\n"); /*get the remaining line*/


        if (hasLabel==TRUE)/*A label definition before an external directive is meaningless*/
        {
            insertToErrorList("Label definition before of .extern directive is meaningless", LineNumber, WARNING);
        }

        if ((Remove_flag= removeSpaces(&current, NORMAL_REMOVE)) != REMOVED_SPACES)/*remove spaces*/
        {
            if (Remove_flag == EXTRA_CONTENT)/* more than one word detected*/
            {
                insertToErrorList("Extrenaous content after label in extetn directive", LineNumber, ERROR);
            }
            else/*empty string*/
            {
                insertToErrorList("No Label detected for .extern Directive", LineNumber, ERROR);
            }
        }

        else/* valid string is given*/
        {
            if((isLabel(current, LineNumber, USAGE) == TRUE)) /*if the string is a valid label */
            {
                tmpSymbol = SearchSymbol(current); /*search the label in the symbols table*/
                if (!tmpSymbol) /*insert the labels as external if not in the tree*/
                {
                    insertToSymbolTable(current, 0, LineNumber, TRUE, FALSE, TRUE); /*external labels adress are set to
 *                                                                                    0 and the symbol is complete*/
                }
                else/*The symbol exist in the symbol table*/
                {
                    if(isExternalSymbol(tmpSymbol)==TRUE) /*if the symbol in the table is defined as external,
 *                                                            it is an error*/
                    {
                        insertToErrorList("Extern directive is already defined!",LineNumber,ERROR);
                    }
                    else /*the symbol is in the table and this is its definition, thus the parameters are fiels*/
                    {
                        SetSymbolAdress(tmpSymbol,0); /*an external label has adress 0*/
                        SetSymbolExt(tmpSymbol, TRUE);/*the symbol is external*/
                        SetSymbolComplete(tmpSymbol, TRUE); /*the symbol is complete*/
                    }
                }
            }
        }
    }
}




/*
 CommandHandler()
-----------------
A method for determining the given line (via the use of strtok_single() method) is a valid command.
 the method is given the command number enumeration, the current line number  and a pointer th the Instructions Counter.
 * */





void CommandHandler(int commandNumber, int LineNumber, int *IC)
{
    char *SourceOperand = NULL; /*a pointer to the scanned would be Source Operand string*/
    char *DestOperand = NULL;/*a pointer to the scanned would be Destination Operand string*/
    char *param1_char = NULL, *param2_char = NULL; /*pointer for holding possible paramter labels for jump type commands*/
    int returnValueSource = FALSE, returnValueDest = FALSE; /*flags for the results of various methods checking the
 *                                                              operands, can be numerical values representing registers
 *                                                              or immediate values*/


    int CurrentStatusFlag = TRUE, SourceFlag = NIL_ADDRESING, DestFlag = NIL_ADDRESING, GroupFlag = TWO_OPERANDS;
    /*flags for the addressing methods associated with the various operands, and a group flag for determining the
 * group of command the current command belongs to:
 *                NO_OPERANDS- only the command is given with no etra content
*                 ONE_OPERAND-the command has only one operand
*                 TWO_OPERANDS-the command has Two operands*/

    char *tmp; /*a temporary char pointer to store a current word while more scanning is done on the given string*/


    int param1 = FALSE, param2 = FALSE, param1_flag = FALSE, param2_flag = FALSE;
    /*addresing flags for the parameters of jump type commands*/

    int Initial_IC = *IC; /*The initial IC*/
    int Remove_flag; /*a flag for the result of space removal from a string via the removespaces method in FirstPass.c*/
    symbol_ptr symbol; /*a symbols pointer  fro interacting with the symbols table*/

    if (commandNumber == JMP_COMMAND || commandNumber == BNE_COMMAND ||
        commandNumber == JSR_COMMAND) /*check if the command belongs to the jump group*/
    {

        DestOperand = strtok_single(NULL,"("); /*the Destination operand is supposed to be  label a  delimited by
 *                                                             "(" */

        if (!(tmp = strtok_single(NULL,",")))/*check if there are no parameters in the command, (tmp is null)*/
        {
             if ((Remove_flag= removeSpaces(&DestOperand, NORMAL_REMOVE)) != REMOVED_SPACES) /*no more content in
 *                                                          the line, thus remove all spaces in the string*/
            {
                CurrentStatusFlag = FALSE;
                if (Remove_flag == EXTRA_CONTENT) /* invalid label with a space in it*/
                {
                    insertToErrorList("Invalid label for jump type command", LineNumber, ERROR);
                }
                else /*empty label*/
                {
                    insertToErrorList("No Label detected for .extern Directive", LineNumber, ERROR);
                }
            }
        }


        else /*more content was found, possible parameters*/
        {
            if ((Remove_flag= removeSpaces(&DestOperand, FIRST_REMOVE)) != REMOVED_SPACES)
                /*remove only the  spaces before the dest operand, if false then:*/
            {
                CurrentStatusFlag = FALSE;
                if (Remove_flag == EXTRA_CONTENT)
                {
                    insertToErrorList("Invalid label for jump type command", LineNumber, ERROR);
                }
                else
                {
                    insertToErrorList("No Label detected for .extern Directive", LineNumber, ERROR);
                }
            }

            if ( hasSpaces(DestOperand)==TRUE) /*after the spaces before the Destoperand were removed, it is possible
 *                                                    that there is space between the label and the "(" delimiter*/
            {
                CurrentStatusFlag = FALSE;
                if(tmp)
                {
                    insertToErrorList("Space detected  in the Label Section of Jump Type command", LineNumber, ERROR);

                }
                else
                {
                    insertToErrorList("Extrenaous content after Jump type command", LineNumber, ERROR);
                }
            }
        }



        if (CurrentStatusFlag==TRUE) /* the space removal so far was ok */
        {
            if (isLabel(DestOperand, LineNumber, USAGE) != FALSE) /*if the label is valid*/
            {
                (*IC) += 2; /*two words are to be created, the instructions word and the label word*/
                symbol = SearchSymbol(DestOperand); /*search the symbols table for the label*/
                if (!symbol) /*if this is a new label, it is inserted and left fot the second pass to copleate*/
                {
                    insertToSymbolTable(DestOperand, *IC, LineNumber, FALSE, FALSE, FALSE);
                    returnValueDest = FALSE;
                }
                else /*the symbol is in the table and its flag is it's compleation status*/
                {
                    returnValueDest = isCompleteSymbol(symbol);
                }


                if (tmp) /*the label was delimited with a "(" with more content ahead*/
                {
                    if (hasSpaces(tmp)==TRUE) /*check that the scanned word up to a comma has no space in it */
                    {
                        insertToErrorList("Space detected  in parameter 1 Section of Jump Type command", LineNumber, ERROR);

                    }
                    else /*parameters were found*/
                    {
                        (*IC) += 2; /*two more words are created, except if both parameters are registers, this is
 *                                                      addressed later on */

                        if ((param1 = isImmediate(tmp, LineNumber)) < MAX_IMMEDIATE_VALUE) /* check if the fist parameter
 *                                                           is an immediate */
                        {

                            if (tmp[0]!='#') /*if it is a number- it must have the # char*/
                            {
                                insertToErrorList("number immediates must be written with the # sign", LineNumber, ERROR);
                            }
                            else /*a valid imediate-set the addresing*/
                            {
                                param1_flag = IMMEDIATE_JMP;
                            }
                        }
                        else if ((param1 = isRegister(tmp)) != FALSE) /*check if the parameter is a register*/
                        {
                            param1_flag = REGISTER_JMP;
                        }

                        else if ((param1 = isLabel(tmp, LineNumber, USAGE)) != FALSE)/*check if the paramete is a label*/
                        {
                            param1_flag = LABEL_JMP;
                            param1_char = tmp;
                            symbol = SearchSymbol(param1_char);
                            if(!symbol)
                            {
                                insertToSymbolTable(param1_char, *IC, LineNumber, FALSE, FALSE, FALSE);
                            }

                        }



                        tmp = strtok_single(NULL, "\n"); /*scan the next parameter*/
                        if (!tmp)
                        {
                            insertToErrorList("No second Operator detected for jump type command", LineNumber, ERROR);

                        }
                        else if (!strchr(tmp,')')) /*search for the ')' delimiter*/
                        {
                            insertToErrorList("')' delimiter found after the second operand", LineNumber, ERROR);
                        }
                        else /*the delimiter was found*/
                        {

                            tmp = strtok_single(tmp, ")"); /*get the word up th the delimiter*/

                            if (hasSpaces(tmp)==TRUE)/*check for spaces up to the delimiter*/
                            {
                                insertToErrorList("Space detected  in parameter 2 Section of Jump Type command",
                                        LineNumber, ERROR);

                            }

                            else/*the second parameter has valid string syntax- no spaces and delimited by a ')'*/
                            {
                                if ((param2 = isImmediate(tmp, LineNumber)) < MAX_IMMEDIATE_VALUE) /*checking if the
 *                                                                      parameter is an immediate*/
                                {
                                    if (tmp[0]!='#')
                                    {
                                        insertToErrorList("number immediates must be written with the # sign",
                                                LineNumber, ERROR);
                                    }
                                    else
                                    {
                                        param2_flag = IMMEDIATE_JMP;
                                    }
                                }
                                else if ((param2 = isRegister(tmp)) != FALSE) /*check if the parameter is a register*/
                                {
                                    param2_flag = REGISTER_JMP;
                                    if (param1_flag==REGISTER_JMP) /*if the  first parameter is also a register than
 *                                                        only 3 words are created*/
                                    {
                                        (*IC) -= 1;
                                    }
                                }
                                else if ((param2 = isLabel(tmp, LineNumber, USAGE)) != FALSE) /*checking if the
 *                                         parameter is a label*/
                                {
                                    param2_flag = LABEL_JMP;
                                    param2_char = tmp;
                                    symbol = SearchSymbol(param2_char);
                                    if(!symbol)
                                    {
                                        insertToSymbolTable(param2_char, *IC, LineNumber, FALSE, FALSE, FALSE);
                                    }
                                }


                                tmp = strtok_single(NULL, "\n"); /*checking if there is more contente after the
 *                                parameters, only spaces are allowed*/
                                if ((removeSpaces(&tmp, FIRST_REMOVE)) != EMPTY_SPACE)
                                {
                                    insertToErrorList("Extrenaous content after Jump type command", LineNumber, ERROR);
                                }
                            }
                        }
                    }
                }

                else/*there are no parameters*/
                {
                    param1_flag=NIL_JMP;
                    param2_flag=NIL_JMP;
                }

               /*insert the flags into the first_pass list as a jump command */
                first_pass_jmp_insert(commandNumber, DestOperand, returnValueDest, param1, param1_flag, param1_char,
                        param2, param2_flag, param2_char, LineNumber, Initial_IC);
            }
        }
    }

    else  /*if the comamnd is not of a jump type*/
    {
        if (commandNumber == STOP_COMMAND || commandNumber == RTS_COMMAND)/*if the command is STOP or RTS*/
        {
            SourceOperand = strtok_single(NULL, "\n"); /*get the sources operand up to the end of the line*/

            GroupFlag = NO_OPERANDS;
            (*IC) += 1; /*only one word is created*/

            if (removeSpaces(&SourceOperand, NORMAL_REMOVE) !=EMPTY_SPACE) /*no more text should be written in the line,
 *                                                                exept for spaces*/
            {
                insertToErrorList("extrenaus content for no operand commands (even spaces)", LineNumber, ERROR);
                CurrentStatusFlag = FALSE;
            }
        }
         /*check if the command is of the ONE_OPERAND group */
        else if (commandNumber == NOT_COMMAND || commandNumber == CLR_COMMAND || commandNumber == INC_COMMAND ||
                 commandNumber == DEC_COMMAND || commandNumber == RED_COMMAND || commandNumber == PRN_COMMAND)
        {

            SourceOperand = strtok_single(NULL, "\n"); /*get the sources operand to the end of the line*/

            GroupFlag = ONE_OPERAND;

            Remove_flag=removeSpaces(&SourceOperand, NORMAL_REMOVE); /*remove spaces for the operand*/

            if(Remove_flag!=REMOVED_SPACES )
            {
                CurrentStatusFlag = FALSE;

                if (Remove_flag==EMPTY_SPACE)
                {
                    insertToErrorList("No dest operand detected for", LineNumber, ERROR);
                }
                else
                {
                    insertToErrorList("Extrenaous content after one operand", LineNumber, ERROR);
                }


            }


        }
         if (CurrentStatusFlag==TRUE && GroupFlag!=NO_OPERANDS) /*if the scan was succsesful so far and the command
 *                                                                      isnt a one operand*/
        {
            if (GroupFlag==TWO_OPERANDS) /*for tow operands command the source operand is delimited by a comma*/
            {

                SourceOperand = strtok_single(NULL, ","); /*get the sources operand*/


                if ((Remove_flag= removeSpaces(&SourceOperand, NORMAL_REMOVE)) != REMOVED_SPACES) /*check space removal*/
                {
                    CurrentStatusFlag = FALSE;
                    if (Remove_flag == EXTRA_CONTENT)
                    {
                        insertToErrorList("No comma detected after first operand of the command", LineNumber, ERROR);
                    }
                    else
                    {
                        insertToErrorList("Coma detected before first operand of command", LineNumber, ERROR);
                    }
                }
            }

            if (CurrentStatusFlag==TRUE) /*if the psace rmoval for two operands was succsesful or the command is
 *                               of one operand only*/
            {
                if ((returnValueSource = isRegister(SourceOperand)) != FALSE) /* check if the source operand is a register*/
                {
                    SourceFlag = REGISTER_ADDRESING;

                }
                else if ((returnValueSource = isImmediate(SourceOperand, LineNumber)) < MAX_IMMEDIATE_VALUE)/*check if
 *                                                             the source operand is an Immediate*/
                {
                    if (SourceOperand[0]!='#') /*in contrast to the data directive, the Immediate must have a #*/
                    {
                        insertToErrorList("number immediates must be written with the # sign", LineNumber, ERROR);
                        CurrentStatusFlag = FALSE;
                    }
                    else
                    {
                        SourceFlag = IMMEDIATE_ADDRESING;
                    }
                }
                else if (isLabel(SourceOperand, LineNumber, USAGE) != FALSE)/*check if the source operand is a label*/
                {

                    SourceFlag = DIRECT_ADDRESING;

                    symbol = SearchSymbol(SourceOperand); /*if it is, search the label in the symbols table, if not,
 *                                                          add it to the table*/
                    if (!symbol)
                    {
                        insertToSymbolTable(SourceOperand, *IC, LineNumber, FALSE, FALSE, FALSE);
                    }
                }
                else /*The source Operand Was Not Recognized*/
                {
                    insertToErrorList("Unrecognized operand", LineNumber, ERROR);
                    CurrentStatusFlag = FALSE;
                }

                /*LEA command is unique and must have a source operand which is a Label*/
                if (CurrentStatusFlag == TRUE && commandNumber == LEA_COMMAND && SourceFlag != DIRECT_ADDRESING)
                {
                    insertToErrorList("Source Operand of LEA command Must be a label", LineNumber, ERROR);
                    CurrentStatusFlag = FALSE;
                }


                if (CurrentStatusFlag == TRUE) /*if the first operand of the command was ok*/
                {
                    /*checking if the command has only one operand*/
                    if (GroupFlag == ONE_OPERAND)
                    {
                       if (SourceFlag==IMMEDIATE_ADDRESING && commandNumber != PRN_COMMAND) /* one operand
 *                                          canot have an immediate as a source operand, exept for the prn command*/
                       {
                           insertToErrorList("Immediate addresing for operand is not accepted for this command",
                                   LineNumber, ERROR);

                           CurrentStatusFlag = FALSE;
                       }
                       else /*one operand commands do not have a source operand but a dest operand, thus the values
 *                                  for the source operand are transfered to the destination operand*/
                       {
                           DestOperand = SourceOperand;
                           DestFlag = SourceFlag;
                           returnValueDest = returnValueSource;

                           SourceOperand = NULL;
                           SourceFlag = NIL_ADDRESING;
                           returnValueSource = FALSE;

                           (*IC)+=2;
                       }
                    }
                    else /*The command is of two operands*/
                    {
                        DestOperand = strtok_single(NULL, "\n"); /*get the rest of the line*/

                        if (strchr(DestOperand,',')) /*No more commas should appear*/
                        {
                            CurrentStatusFlag=FALSE;
                            insertToErrorList("Extra comma ( ',' ) detected after second operand", LineNumber, ERROR);

                        }
                            /*space removal for the dest operand*/
                        else  if ((Remove_flag= removeSpaces(&DestOperand, NORMAL_REMOVE)) != REMOVED_SPACES)
                        {
                            CurrentStatusFlag = FALSE;
                            if (Remove_flag == EXTRA_CONTENT)
                            {
                                insertToErrorList("Extrenaous content after Second Operand of command", LineNumber, ERROR);
                            }
                            else
                            {
                                insertToErrorList("No second Operand Detected", LineNumber, ERROR);
                            }
                        }
                    }

                }
                /*if the command is of two operands*/
                if (CurrentStatusFlag == TRUE && GroupFlag == TWO_OPERANDS)
                {
                    (*IC) += 3; /*The total number of words for Two operands are 3, with the eception of two register
 *                     operand which is handeled later on*/


                    /*Now  the Destination operand is cheacked similiraly to the source operand to determine
                     *what it is */

                    if ((returnValueDest = isRegister(DestOperand)) != FALSE) /* check if the destination operand is a
 *                       register*/
                    {
                        DestFlag = REGISTER_ADDRESING;
                        if (SourceFlag == REGISTER_ADDRESING)
                        {
                            (*IC)--;
                        }
                    }
                    else if ((returnValueDest = isImmediate(DestOperand, LineNumber)) < MAX_IMMEDIATE_VALUE)

                    {
                        if (DestOperand[0]!='#')
                        {
                            insertToErrorList("number immediates must be written with the # sign", LineNumber, ERROR);
                            CurrentStatusFlag = FALSE;
                        }
                        else  if (commandNumber != CMP_COMMAND)/* The compare command-is the only command that allows for an immediate
 *                                                 second operand*/
                        {
                            insertToErrorList("Second operand of command cannot be an Immediate", LineNumber, ERROR);
                            CurrentStatusFlag = FALSE;
                        }
                        else
                        {
                            DestFlag = IMMEDIATE_ADDRESING; /*cmp command can have immediate destoperand addressing*/
                        }


                    }
                    else if (isLabel(DestOperand, LineNumber, USAGE) != FALSE) /*if the Destination Operand is a label*/
                    {

                        DestFlag = DIRECT_ADDRESING; /*set the label addresing*/

                        symbol = SearchSymbol(DestOperand); /*search the symbols table for the label*/
                        if (!symbol) /*if this is a new label, it is inserted and left fot the second pass to copleate*/
                        {
                            insertToSymbolTable(DestOperand, Initial_IC, LineNumber, FALSE, FALSE, FALSE);
                            returnValueDest = FALSE;
                        }
                        else
                        {
                            returnValueDest = isCompleteSymbol(symbol);
                        }
                    }
                    else
                    {
                        insertToErrorList("Second operand of command was not recognized as a valid operand",
                                          LineNumber, ERROR);
                        CurrentStatusFlag = FALSE;
                    }
                }

                /*The case for the Load Effective Adress Command- the dest operand must be a label or a register*/
                if (CurrentStatusFlag == TRUE && commandNumber == LEA_COMMAND && DestFlag==IMMEDIATE_ADDRESING)
                {
                    insertToErrorList("Dest Operand of LEA command Must be a label or a register", LineNumber, ERROR);
                    CurrentStatusFlag = FALSE;
                }

            }



        }
        if (CurrentStatusFlag == TRUE) /*The scan is finished and the flags are inserted into the first pass list*/
        {
            first_pass_command_insert(commandNumber, SourceOperand, SourceFlag, returnValueSource, DestOperand,
                                      DestFlag, returnValueDest, LineNumber, Initial_IC);
        }
    }
}


/*Auxillary methods */

/*
 isRegister()
-----------------
a method for checking if a current scanned string represents a register, com
 Gets a string and compares it the the strings in  the REGISTERS array, if it matches a string, its cell number, which
 corresponds to the register numer.  If not match is found - FALSE (-1)is returned.
 * */


int isRegister(char * str)
{
    int i;
    for(i=0;i<NUM_OF_REGISTERS;i++)
        if (strcmp(str,REGISTERS[i])==0)
            return i; /*return the number of the command*/
    return FALSE;
}




/*
 isImmediate()
-----------------
a method for checking if a current scanned string represents  a valid immediate.
 it can distinguish between .data directive numbers which are given as is, versus command Immediates, which are given
 with a # symbol before the number, e.g #-5. The size of the numbers is defined by the 12 size limitation of the
 command Imediatats, since the numbers in the data can interact with outer operands in the assembly language.

 The functions returns the integer value of the number in ths string, and thous if a value greater than the allowed
 maximal value is returned- the given string did not contain a valid number.

 * */

int isImmediate(char * str,int LineNumber)
{
    char *endptr = NULL; /*the next character in the given string after the numerical value*/
    int num;

    if (str[0]=='#') /*This is a command Immediate, */
    {
        num = strtol(str+1, &endptr, BASE); /*using the strtol to convert the possible numerical string to a base 10
 *                                           (defined at the gegining of the file )number*/
    }
    else
    {
        num = strtol(str, &endptr, BASE);
    }

    if (*endptr != '\0') /*The string was not a valid number, only the end of the string is considered as valid result*/
    {
        /*add an error and exit function*/
        num=MAX_IMMEDIATE_VALUE+1;
    }
    else if ((num < MIN_IMMEDIATE_VALUE) || (num > MAX_IMMEDIATE_VALUE)) /*checking if the scanned number is within the
 *                                          size limits*/
    {
        /*add error and exit function*/
        insertToErrorList("The number exceedes size limits", LineNumber, ERROR);
        num=MAX_IMMEDIATE_VALUE+1;
    }
    return num;
}

/*
  *  hasSpaces()
 -----------------
 A simple method that scans a given string and returns true if a space was detected within it.
 * */

bool hasSpaces(char * str)
{
    int n=strlen(str);
    int i;
    for (i=0;i<n;i++)
    {
        if (isspace(str[i]))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*
  *  hasSpaces()
 -----------------
 A simple method that scans a given string and returns the number of commas within it.
 * */
int getCommaNum(char* str)
{
    int i;
    int n;
    int count=0;
    n=strlen(str);
    for (i=0;i<n;i++)
    {
        if (str[i]==',')
        {
            count++;
        }
    }
    return count;
}