/*
 *
 * * Course No. 20465- Final Project
 * Student:  Francois Oren Chikli
 *
 FINAL PROJECT
===========

SymbolTable.c

Short Description:
------------------
A file defining the SymbolTable data structure, which is a red black binary search tree.


The implementation of the method is taken from the course book of the course "Introduction to data structures and
 algorithms - 20407"

 This Data Structure was chosen for the symbols table since throughout the entire program, many different methods
 apply the searching method to find and return a node in the structure. This data Structure preforms the
 search in O(log(n)) time complexity which is optimal.

Also, only a selectef few methodes are implemented for this use. For example,
 No node deletion is implemented since it is not needed.

 Since this is not a course about algorithms, I assume that the inner workings of the base algorithms for this data
 structure are known, thus minimal explanations are given on the maxinstances algoritem InsertFixup()
which requires references to the properties of the red black binary search tree, which i belive is beyond the scope
 of this course.


----------------------------------------
 */

#include "SymbolTable.h"
#include "ErrorList.h"



#define GrandParent(x) ((x)->p->p) /*a macro for geting a nodes grandparant*/
#define RightUncle(x) ((x)->p->p->right)/*a macro for geting a nodes  right uncle- the right son of its parent*/
#define LeftUncle(x) ((x)->p->p->left)/*a macro for geting a nodes  left uncle- the left son of its parent*/
#define isLeftSon(x) ((x)==(x)->p->left) /*a method for checking if a current node is the left son*/
#define isRightSon(x) ((x)==(x)->p->right)/*a method for checking if a current node is the right son*/




 /*Internal method for maintaining the table*/
void Update(symbol_ptr,int); /*Update the addresses of a given pointer*/
symbol_ptr Search(symbol_ptr, char*); /*Search a symbole*/
void insertNode(symbol_ptr *);
void InsertFixup(symbol_ptr* );
void RotateLeft(symbol_ptr );
void RotateRight(symbol_ptr );
symbol_ptr createNode(char *symbol, int ADDRESS, int LineNumber, bool EXT, bool OP, bool complete);

typedef enum {BLACK,RED}colors; /*define the node colors used in the node structure*/

symbol_ptr find(symbol_ptr node);


/*a structure defining a node of the tree */
struct node{

    char label[MAX_SYMBOL_SIZE]; /*The symbol*/
    int address; /*The address of the symbol*/
    int LineNumber; /*The  Line number where the label was first found*/
    bool isExternal; /*is the label external*/
    bool isOperation;/*is the label copuled with an operation */
    bool isComplete; /*is the Label Complete*/

    colors node_color; /*The nodes color*/
    symbol_ptr p; /*The nodes parent*/
    symbol_ptr left; /*The nodes left son*/
    symbol_ptr right;/*The nodes right son*/
};


symbol_ptr NIL=NULL; /*The Tree NIL pointer which replaces the NULL definition in a red black tree*/
symbol_ptr ROOT=NULL; /*The root of the tree*/


void initializeSymbolTable() /*Initialize the Tree by creating the first node- the NIL node which is also the root*/
{

    NIL = (symbol_ptr) malloc(sizeof(struct node));/*allocate space for the node*/

    if(NIL==NULL)
    {
        fprintf(stderr,"Memory allocation failure for the symbols table");
        exit(0);
    }

    /*set value for node variable*/
    strcpy(NIL->label,"NIL");
    NIL->node_color = BLACK;
    NIL->left = NIL;
    NIL->right = NIL;
    ROOT=NIL;/*The tree is empty and begins from the NILL pointer*/


}


/*
 insertToSymbolTable()
-----------------
A method for inserting values into the table and creating a node, it receives the node values, and uses the internal
 function insertNode to insert the created node into the tree.
 * */


void insertToSymbolTable(char *symbol, int ADDRESS, int LineNumber, bool EXT, bool OP, bool complete)
{
    symbol_ptr tmp;
    /*initialize the Symbol Table if it the first insert*/
    if (NIL==NULL && ROOT==NULL)
        initializeSymbolTable();

    tmp= createNode(symbol, ADDRESS, LineNumber, EXT, OP, complete); /*create a node*/
    insertNode(&tmp);

}


/*
 createNode()
-----------------
A method for creating a tree node and setting its values to the recived values
 * */

symbol_ptr createNode(char *symbol, int ADDRESS, int LineNumber, bool EXT, bool OP, bool complete)
{
    symbol_ptr tmp;
    tmp =(symbol_ptr)malloc(sizeof(struct node)); /*allocate space for the node*/
    if(!tmp)
    {
        fprintf(stderr,"Error Allocatin a node in the  Symbols Table\n");
        exit(1);
    }

    /*set the values of the node to the recived values*/
    strcpy(tmp->label,symbol);
    tmp->address=ADDRESS;
    tmp->LineNumber=LineNumber;
    tmp->isExternal=EXT;
    tmp->isOperation=OP;
    tmp->isComplete=complete;

    tmp->p=NULL;
    tmp->left=NULL;
    tmp->right=NULL;
    return tmp;
}


/*
 insertNode()
-----------------
A method for inserting a node into the symbols table, this is a direct implementation of the  RB-INSERT method in
 p236 of  the course book of the course "Introduction to data structures and algorithms - 20407"
 * */
void insertNode(symbol_ptr *node)
{
    /*create pointers to the nodes as shown in the book*/
    symbol_ptr z=*(node); /*a pointer to the given node*/
    symbol_ptr y=NIL; /*a pointer to the parent of the node x which wil run throught the tree */
    symbol_ptr x=ROOT; /*The beginning of the tree traversal until a leaf is found*/

    while (x!=NIL) /*scan thought the tree*/
    {
        y=x;/*y is set to be x since x will become one of its children*/

        if(strcmp(z->label,x->label)>0) /*if the symbol in the given node Z is smaller than the one in the left son
 *                                         set x to be the left son*/

            x=x->left;
        else /*the label value of z is smaller the x, set x to be the right son*/
            x=x->right;
    }
    z->p=y;/*when doen, x is NILL and thus y is bo be the new parent of x*/
    if(y==NIL) /*The tree is empty- set the node as the root*/
        ROOT=z;
    else if(strcmp(z->label,y->label)>0) /*if the label is smaller in value than y, the node is the left son*/
        y->left=z;
    else /*the node is the right son if y*/
        y->right=z;
    /*The node is now a leaf and so its values are as follows:*/
    z->left=NIL;
    z->right=NIL;
    z->node_color=RED;
    /*Maintain the red black property of the tree via the InsertFixup() method */
    InsertFixup(node);
}

/*
 InsertFixup()
-----------------
A method for maintaining the red black tree properties after the insertion of the the node to the red black tree,
 a direct implementation of the RB-InsertFixUp method in  p236 of  the course book of the course
 "Introduction to data structures and algorithms - 20407"

 After performing this method the tree structure is changed to become a balanced tree, allowing for the symbol search
 in in the tree to be of O(log(n)) time complexity.

 Since this method requires explanations about the properties of the red black tree it is remained without any internal
 documentation.

 * */

void InsertFixup(symbol_ptr* node)
{
    symbol_ptr z=*(node);
    symbol_ptr y;
    while ((z->p->node_color)==RED)
    {
        if (isLeftSon(z->p))
        {
            y=RightUncle(z);
            if((y->node_color)==RED)
            {
                (z->p->node_color)=BLACK;
                y->node_color=BLACK;
                (GrandParent(z)->node_color)=RED;
                z=GrandParent(z);
                *(node)=z;
            }
            else
            {
                if (isRightSon(z))
                {
                    z=z->p;
                    *(node)=z;
                    RotateLeft(*node);
                }
                z->p->node_color=BLACK;
                (GrandParent(z)->node_color)=RED;
                RotateRight(GrandParent(z));
            }
        }
        else if (isRightSon(z->p))
        {
            y = LeftUncle(z);
            if ((y->node_color) == RED)
            {
                (z->p->node_color) = BLACK;
                y->node_color = BLACK;
                (GrandParent(z)->node_color) = RED;
                z = GrandParent(z);
                *(node)=z;
            }
            else
            {
                if (isLeftSon(z))
                {
                    z=z->p;
                    *(node)=z;
                    RotateRight(*node);
                }
                z->p->node_color=BLACK;
                (GrandParent(z)->node_color) = RED;
                RotateLeft(z->p->p);
            }

        }

    }
    ROOT->node_color=BLACK;
}



/*
 RotateLeft()
-----------------
A method for left rotation of a given node- switching it (denoted as x) with its left son ( detonated y)
 while maintaining the tree red black properties.
 This is  used  during the maintenance of the tree in the InsertFixup and is a direct implementation of the
 LEFT-ROTATE method in  p234 of  the course book of the course "Introduction to data structures and algorithms - 20407"
 * */

void RotateLeft(symbol_ptr x)
{
    symbol_ptr y=x->right; /*set y to be the right son of x*/
    x->right=y->left;/*set x to be the left son of y*/

    if (y->left!=NIL) /*if the left son of y isnt NILL set it to be the parent of x*/
        y->left->p=x;

    y->p=x->p; /*set the parent of y to be the parent of x*/

    if(x->p==NIL) /*if the parent of x is NIL- it was the root of the tree, thus now y is the */
        ROOT=y;
    else if (isLeftSon(x)) /*if x was the left son of its parent set y to be the left son of x's parent*/
        x->p->left=y;
    else /*x was the right son of  its parent, thus set y to be the right son of its parent*/
        x->p->right=y;
    y->left=x; /*y was the left son of x so now x is the left son of y*/
    x->p=y;/*y is now the parent of x*/
}

/*
 RotateLeft()
-----------------
A method for right rotation of a given node, compleatly analogous to the  RotateLeft method.
 * */


void RotateRight(symbol_ptr x)
{
    symbol_ptr y=x->left;
    x->left=y->right;

    if (y->right!=NIL)
        y->right->p=x;

    y->p=x->p;

    if(x->p==NIL)
        ROOT=y;
    else if (isRightSon(x))
        x->p->right=y;
    else
        x->p->left=y;
    y->right=x;
    x->p=y;
}

/*
SearchSymbol()
-----------------
A method for searching a symbol in the SymbolTable, it calls upon the  Search method with the initial value
 of the tree ROOT.
 * */

symbol_ptr SearchSymbol(char* symbol)
{
    return Search(ROOT,symbol);

}
/*
SetSymbolAdress()
-----------------
A method for changing the given sybols pointer adress to the given one
 * */

void SetSymbolAdress(symbol_ptr Symbol, int add)
{
   Symbol->address=add;
}

/*
SetSymbolAdress()
-----------------
A method for changing the given symbols pointer completion status to the given one
 * */

void SetSymbolComplete(symbol_ptr Symbol, bool isComplete)
{
    Symbol->isComplete=isComplete;
}

/*
SetSymbolAdress()
-----------------
A method for changing the given symbols pointer external status to the given one
 * */

void SetSymbolExt(symbol_ptr Symbol, bool isExt)
{
    Symbol->isExternal=isExt;
}

/*
SetSymbolAdress()
-----------------
A method for changing the given symbols pointer operation coupeling status to the given one
 * */

void SetSymbolOP(symbol_ptr Symbol, bool isOP)
{
    Symbol->isOperation=isOP;
}


/*
Search()
-----------------
A method for searching a symbol in the SymbolTable, an adaptation of the Tree-Search Method in p234
 of  the course book of the course "Introduction to data structures and algorithms - 20407"
 * */

symbol_ptr Search(symbol_ptr x,char* symbol)
{
    int strcmpReslt; /*an integer to get the result of the strcmpr function checking the given symbols value agains
 *                      the one of the current node searched*/
    while ((x!=NIL) && ((strcmpReslt=strcmp(symbol,x->label))!=0)) /*search the tree as long as the current node is
 * not NULL and the result of strcmp is zero, meaning the symbol was found in the list.*/
    {
        if(strcmpReslt>0) /*the symbol value is smaller than the one of the current node*/
            x=(x->left);
        else /*the symbol value is greater than the one of the current node*/
            x= (x->right);
    }
    if (x==NIL) /*The symbol was not found, retun NULL to the calling method*/
    {
        x=NULL;
    }

    return x;
}

/*
UpdateSymbols()
-----------------
A method for updating the Address of the symbols which are related to .data/. string directive.
 The methode is given the final IC Which is added to the corespoindin Symbols adress. The method calls
 the internal Upadte Function WIth the root  of th tree.
 * */

void UpdateSymbols(int IC)
{
    Update(ROOT,IC);
}


/*
Update()
-----------------
A method for updating the Address of the symbols which are related to .data/. string directive.
 The methode is given the final IC Which is added to the corespoindin Symbols adress.
 This is a reursive method, at each step a node is checked if it is coupled  with the  .data/. string directives,
 if it is, its adress is updated.
 After the node is cheaked, its left and right sons are iterativly called.
 * */

void Update(symbol_ptr node, int IC)
{

    if (node!= NIL)
    {
        if (node->isComplete==TRUE &&node->isExternal == FALSE && node->isOperation == FALSE)
        {
            node->address += IC;
        }
        Update(node->left,IC);
        Update(node->right,IC);
    }

}


/*
 find_missing()
-----------------
An external method for returning a pointer to the first symbol that was found to be incomplete in the table,
 meaning if was not defined.
 The method calls upon the internal method find().

 * */

symbol_ptr find_missing()
{
    return find(ROOT);
}




/*
 find()
-----------------
An internal method for returning a pointer to the first symbol that was found to be incomplete in the table,
 meaning if was not defined.
 The method scans the tree until a node is found that is incomplete.
 The method is called upon at the end of the firstPass method (defined in the FirstPass.c file) to return all the
 incomplete symbols. Thus when A symbol is found, its isComplete flag is set to True so it will not be returned again.


 * */

symbol_ptr find(symbol_ptr node)
{
    symbol_ptr tmp;
    if (node!=NIL )/*iterate through the tree as long as the NILL node is note encountered*/
    {
        if ( node->isComplete==FALSE) /* if the node is not complete- set its complet to TRUE for the next
 *                                          time the find_missing function is called*/
        {
            node->isComplete=TRUE;
        }
        else if (!(tmp=find(node->left)))/*run the method on the left son*/
        {
            node=(find(node->right)); /*if not found run on the right sub tree and get the*/
        }
        else /*a node was found in the left sub tree*/
        {
            node=tmp;
        }
    }
    else if (node==NIL) /*return NULL to the calling function*/
    {
        node= NULL;
    }
    return node;
}






/*
 SymbolLineNum()
-----------------
A method for returning the symbol address

 * */

int SymbolLineNum(symbol_ptr label_node)
{
    return label_node->LineNumber;
}



/*
 isExternalSymbol()
-----------------
A method for returning the flag signaling if the symbol is complete

 * */
bool isCompleteSymbol(symbol_ptr symbol_node)
{
    return symbol_node->isComplete;
}



/*
 SymbolAddress()
-----------------
A method for returning the given nodes adreess

 * */

int SymbolAddress(symbol_ptr label_node)
{
    return label_node->address;
}

/*
 isExternalSymbol
-----------------
A method for returning the flag signaling if the symbol is external

 * */
bool isExternalSymbol(symbol_ptr symbol_node)
{
    return symbol_node->isExternal;
}




/*
 FreeSymbolTable()
-----------------
A  recuursive method for freeing the tree. It usest two tmeporary pointer, lt and rt to hold the sons of a current
 node, frees the node via the free() methods, and reccrsivly runs on the sons.
*/

void FreeTable(symbol_ptr root)
{
    symbol_ptr lt, rt;
    if (root!= NIL)
    {
        lt=root->left;
        rt=root->right;
        free(root);
        FreeTable(lt);
        FreeTable(rt);

    }
}
/*
 FreeSymbolTable()
-----------------
An external method for freeing the tree. The method calls upon the internal method FreeTable() with the root of the
 tree and freese the root and NIL nodes after it's compleation
*/
void FreeSymbolTable()
{
    FreeTable(ROOT);
    free(NIL);
    ROOT=NULL;
    NIL=NULL;
}



