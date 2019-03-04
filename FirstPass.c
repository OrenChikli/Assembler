/*
 *
 * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 * FINAL PROJECT
 * ===========
 *
 * FirstPass.c
 *
 * Short Description:
 * ------------------
 * A file containing several function for the "firstPass" of a given file
 *
 * Assumptions:
 * A valid file pointer of an "file.as" file is given.
 *
 * ----------------------------------------

 */

#include "FirstPass.h"
#include "GlobalDeclarations.h"

#include "FirstPassList.h"
#include "DataList.h"
#include "ErrorList.h"
#include "SymbolTable.h"
#include "FirstPassHandlers.h"
#include "EntryList.h"
#include "ExternList.h"

#define isComment(x) (((x)[0])== ';') /*a macro for checking if a scanned line is a comment*/
#define NUM_OF_COMMANDS 16 /*The number of Commands used in the assembler Syntax*/
#define NUM_OF_DIRECTIVES 4 /*The number of different directives defined in the assembler syntax*/

char* COMMAND_STRING[NUM_OF_COMMANDS]={"mov","cmp","add","sub","not","clr","lea","inc",
                                       "dec","jmp","bne","red","prn","jsr","rts","stop"};
/*An array of strings containing the command name in the order in which is shown in the table at p.15-16 of the
 * Course Manual. thus each string is located at the cell number corresponding with its numerical code.
 * e.g mov is in cell No.-0 and sub is in cell No.-3*/


char* DIRECTIVES[NUM_OF_DIRECTIVES]={".data",".string",".entry",".extern"};
/*An array of strings containing a directive string, in the order corresponding the the Dirctives enumeration  in the
 * firstPassHandler.h file */

int isDirective(char *); /*An internal method for determining if a given string is a Directive*/
int isCommand(char *);   /*An internal method for determining if a given string is a Command*/





/*
 FirstPass()
-----------------
Get a file pointer to the current analysed file and its name.
The method than scans the file ,each row at a time  and analyses the row to pruduce an First_pass list containing
information to be later printed as assembler words.
 * */



void FirstPass(FILE *fp, int *IC, int *DC)
{

    char line[MAX_LINE_SIZE]; /* an array to hold the current line in the file*/
    char *currentScannedWord; /*a pointer to the current analyzed word*/
    char* char_tmp; /*A temporary char pointer for use in some situations explained later*/
    char* line_tmp; /*A char pointer created only due to my IDE's warning for possible type mismatch explained later on*/
    int LineNumber = 0; /*an index specifying the current line*/
    int StatusFlag; /* A flag for the return values from the various methods used, with which decisions can be made */
    int n; /*An integer used for getting the current line lenght, used for determining if line exceeds limits*/
    char c; /* a character use for the case that a line excedes size limits*/
    symbol_ptr tmpSymbol; /*A temporary Symbols tree pointer for later use*/


    while (fgets(line, MAX_LINE_SIZE+1, fp))/* scan all the line in the file, at each iteration
 * a line is scanned, with 1 extra bit from the maximum allowed length*/
    {
        LineNumber++;/*line was successfully scanned thus the index is incremented */

        line_tmp=line; /*using the line_tmp since the IDE states that &line is not of char** type but of char*[81]
 * which from my point of view is allowed*/

        /*This coindition checks if the line is empty or a comment (finding ;)*/
        if (isComment(line) || removeSpaces(&line_tmp, FIRST_REMOVE) == EMPTY_SPACE)
        {
            continue;
        }


        n=strlen(line); /*geting the actuall length of the string*/
        if(n==MAX_LINE_SIZE) /*since line is MAX_LINE_SIZE+1  in length, if n==MAX_LINE_SIZE than the line in the file
 * is longer than permitted*/
        {
            insertToErrorList("Line exceeded the length limit", LineNumber, ERROR);

            /*Since the line is longer than premited, the file pointer needs to move upto the next line*/
            c=(char)getc(fp);
            while(c!='\n')
            {
                c=(char)getc(fp);
            }
            continue;
        }





        /*if the line is not a comment ,get The First word using the strtok_single method of the scanned line*/
        currentScannedWord = strtok_single(line_tmp, " \t\n");

        /*if the first scanned word is a directive, move on to the directive handler of the FirstPassHandlers*/
        if ((StatusFlag=isDirective(currentScannedWord)) != FALSE)
        {
            DirectivesHandler(StatusFlag, LineNumber, DC, FALSE, IC);
        }

            /*if the first scanned word is a command, move on to the commands handler of the FirstPassHandlers*/
        else if ((StatusFlag=isCommand(currentScannedWord)) != FALSE)
        {
            CommandHandler(StatusFlag, LineNumber, IC);
        }

        /*if the first scanned word is valid label, then this is its definition (thus the isLabel function recives the
         * DEFINITION enum), and the program moves on to check if the label is present at the symbols table or not,
         * and checks if the next word is a command-meaning its addres is final for this instance (relocatable*/
        else if (isLabel(currentScannedWord, LineNumber, DEFINITION) != FALSE) /*This is the definition of the label*/
        {
            char_tmp=currentScannedWord; /*Save the label until more information about it is recived*/
            tmpSymbol=SearchSymbol(char_tmp); /*search the label in the symbols table*/

            currentScannedWord = strtok_single(NULL, "\n"); /*scan the second word*/
             /*Remove the space before the string- if the string is NULL or made only from spaces than no input is given*/
            if (removeSpaces(&currentScannedWord, FIRST_REMOVE) == EMPTY_SPACE)
            {
                insertToErrorList("No input detected after label Definition", LineNumber, ERROR);
                continue;
            }

            /*The first word after a label is seperated for the rest of the words with a space and since the program got
             * to this point- there is asring here.*/
            currentScannedWord = strtok_single(currentScannedWord, " \t");

            if((StatusFlag=isCommand(currentScannedWord)) != FALSE)
            {

                if (!tmpSymbol) /*meaning the symbol is not in the symbols table-insert it*/
                {
                    insertToSymbolTable(char_tmp, *IC, LineNumber, FALSE, TRUE, TRUE); /*This is a an internal label
                                                              * signaling an operationand so this label is complete*/
                }

                else /*the the symbol is already in the symbols table**/
                {
                    if (isExternalSymbol(tmpSymbol)==TRUE) /*if the label is external the definition is an error*/
                    {
                        insertToErrorList("Defining a label which is External", LineNumber, ERROR);
                        continue;
                    }
                     /*Update the symbols parameters*/
                    SetSymbolAdress(tmpSymbol, *IC);
                    SetSymbolComplete(tmpSymbol, TRUE);
                    SetSymbolOP(tmpSymbol, TRUE);
                }

                CommandHandler(StatusFlag, LineNumber, IC); /*encode the command that follows*/
            }

            else if((StatusFlag=isDirective(currentScannedWord)) != FALSE)/*if the scanned word is a directive*/
            {

                if (!tmpSymbol) /*if the symbol is not in the symbols table*/
                {
                    insertToSymbolTable(char_tmp, *DC, LineNumber, FALSE, FALSE, TRUE);
                }
                else /*Update the symbol parameters*/
                {
                    SetSymbolAdress(tmpSymbol, *DC);
                    SetSymbolComplete(tmpSymbol,TRUE);
                }

                DirectivesHandler(StatusFlag, LineNumber, DC, TRUE, IC); /*move on to handel the directive*/
            }
            else /*The word was not recognized*/
            {
                insertToErrorList("Unrecognized second operator", LineNumber, ERROR);
            }


        }
    }

    UpdateSymbols(*IC); /* update addresses  Symbols Table*/



    if ((tmpSymbol=find_missing())) /*after the updates tit is possible to look for undfined labels- thus that are
 * note writen as first words or as .externals*/
    {
        while(tmpSymbol)
        {
            insertToErrorList("Undeclared Symbol", SymbolLineNum(tmpSymbol), ERROR);
            tmpSymbol=find_missing();
        }
    }
    else /*No undefined labels were found- the program can continue */
    {
        UpdateDataAddress(*IC); /*Update the adresses of the Data labels*/
        UpdateEntryList();/*Update the EntryList*/
    }

}


/*
 isDirective()
-----------------
Gets a string and compares it the the strings in  the DIRECTIVES array, if it matches a string, its cell number, which
 corresponds to the directive type enum in DIRECTIVE_ENUM  in the header file is returned.  If not match is found - FALSE (-1)
 is returned.

 * */
/*Test if given string is a directive*/
int isDirective(char * str)
{
    int i;
    for(i=0;i<NUM_OF_DIRECTIVES;i++)
        if (strcmp(str,DIRECTIVES[i])==0)
            return i; /*return the number of the directive*/
    return FALSE;
}


/*
 isCommand()
-----------------
Gets a string and compares it the the strings in  the COMMAND_STRING array, if it matches a string, its cell number, which
 corresponds to the comand enum in COMMAND_ENUM  in the header file is returned.  If not match is found - FALSE (-1)
 is returned.

 * */
/*Test if given string is a directive*/

int isCommand(char * str)
{
    int i;
    for(i=0;i<NUM_OF_COMMANDS;i++)
        if (strcmp(str,COMMAND_STRING[i])==0)
            return i; /*return the number of the command*/
    return FALSE;
}

/*
 isLabel()
-----------------
Gets a string and check if it meets the requirments for a valid string, given the strings possible status,
 according to the type, a DEFINITION for possible label definition of USAGE for possible usage as an operand

 * */

int isLabel(char *CurrentStr, int lineNumber, labelType type)
{
    int i;
    int len=strlen(CurrentStr);
    int ReturnFlag=TRUE;

    if (!isalpha(CurrentStr[0])) /*check if a label starts with a letter*/
    {
        insertToErrorList("A label must start with a Letter", lineNumber, ERROR);
        ReturnFlag=FALSE;
    }

    else if(type==DEFINITION &&CurrentStr[len-1]!= ':') /*if it is a definition- it must have : at the end*/
    {
        insertToErrorList("Label must be followed by \":\" character", lineNumber, ERROR);
        ReturnFlag=FALSE;
    }

    else if(type==DEFINITION) /*The string is being defined, thus the : is removed*/
    {
        len--;
        CurrentStr[len]= '\0';
    }

    else if(type==USAGE && CurrentStr[len-1]== ':') /*if a string is used, i cannot have : at the end*/
    {
        insertToErrorList("Label usage must be without the \":\" character", lineNumber, ERROR);
        ReturnFlag=FALSE;
    }

    else /*checking that a label is made only from letters and digits*/
    {

        if (len>MAX_SYMBOL_SIZE) /*checking if the label is within the length requirments*/
        {
            insertToErrorList("Label name too long", lineNumber, ERROR);
            ReturnFlag=FALSE;
        }
        else
        {
            i=1;
            while (ReturnFlag==TRUE && i<len) /*if each character is scanned properly*/
            {
                if (!isalpha(CurrentStr[i]) && !isdigit(CurrentStr[i])) /*check if the current char is aletter or a
 * digit*/
                {
                    insertToErrorList("A label must be comprised form letters and digits only", lineNumber, ERROR);
                    ReturnFlag=FALSE;
                }
                i++;
            }
        }

    }

    return ReturnFlag;
}


/*
 strtok_single()
-----------------
User defined version of strtok() which unlike the original version, returns WHATEVER is found upto a given delimiter,
 even if it is NULL or only empty space.

 * */

char *strtok_single(char *str, char const *delims)
{
    static char  * src = NULL; /*a static arry for furthe usages of the method, hold the rest of the string after the
 * delimiters*/
    char  *  p,  * ret = 0;/*p-  a current pointer which scanes the file
 *                           ret- the string that will be returned to the user*/

    if (str != NULL)/*if the Uuser inputed NULL, than the scan continues for the saved string*/
        src = str;

    if (src == NULL || src[0]=='\n')/*return null if the string is NULL or the end of the line*/
        return NULL;

    else if ((p = strpbrk (src, delims)) != NULL) /*use the strpbrk that returns a pointer to the first appearens
 * of the delimiter*/
    {

        *p  = 0; /*set the delimiter to 0*/
        ret = src; /*now the string in src up to the delimiter is what needed an will be returned*/
        src = ++p; /*move to the rest of the string*/
    }
    else if (*src) /*if the delimiter was not found return the string and set the reminder to NULL*/
    {
        ret = src;
        src = NULL;
    }


    return ret;
}

/*
 removeSpaces()
-----------------
A method for removing spaces form a given string by the case it was given as described in the header:
 * FIRST_REMOVE- removes only the spaces up to the first non space char
 * NORMAL_REMOVE-removes all the spaces found in the string.
 *
 * The method returns the a flag as follows:
 *
 * * EMPTY_SPACE- the string was found to be null or contained only whitespaces
 * EXTRA_CONTENT- a non space char was found after a string, meaning two "words" were found in the string
 * REMOVED_SPACES}- the space removal was succsesful

 * */

int removeSpaces(char **str, int RemoveCase)
{
    char *p; /*a char pointer for scanning the string*/
    int ReturnFlag=REMOVED_SPACES; /*a return flag*/

    if (*str==NULL)/*if the given string is empty*/
    {
        ReturnFlag=EMPTY_SPACE;
    }
    else
    {
        p=*str;/*set the pointer to the begining of the string */
        while((*p)!='\"' && isspace(*p))/*scans the first part of the string for spaces*/
        {
            p++;
        }

        if (p[0]=='\0')/*The end of the string is reached- only white spaces were found */
        {
            ReturnFlag=EMPTY_SPACE;
        }


        else /*a non white space char was found*/
        {
            *str=p; /*"remove" the spaces, reset the pointer to the first non space char*/
            if (RemoveCase!=FIRST_REMOVE) /*for the FIrst Remove cas no thurther action is needed*/
            {
                while(!isspace(*p) && *p!='\0')/*now scan the charecters until the end of the string of the first space*/
                {
                    p++;
                }
                if (*p!='\0') /*if the pointer is not '\0' than there is more to the string*/
                {
                    *p=0;
                    p++;

                    while(isspace(*p)) /*scann the rest of the string*/
                    {
                        p++;
                    }

                    if (*p!='\0') /*if the end of the string is not reached than there are two "words" in the string*/
                    {
                        ReturnFlag=EXTRA_CONTENT;
                    }
                }
            }
        }
    }

    return ReturnFlag;
}




