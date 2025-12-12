#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************************************************************************************************
* Include this file in a file where you will define everything , unless indicated otherwise by a comment, that is declared as extern in this file. *
* Include ExpressionParser.c in the file where you will actually use the parser; before using the parser must set up with:                         *
operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work                                                                 *
checkOperatorAndOperandCharsDefinitions(); //to check there's no errors in the definitions                                                         *
* Customize the tests in runExpressionTests() defined in ExpressionParser.c .                                                                      *
***************************************************************************************************************************************************/

/******************************************************
* IMPORTANT!                                          *
* This define is the only thing to edit in this file. *
******************************************************/
#define OPERAND_VALUE_TYPE float

typedef struct Operand{
	OPERAND_VALUE_TYPE value;
} Operand;

/***********************************************************************
* IMPORTANT!                                                           *
* Needed for parseOperandStringFormatToVoidPtr(char *s) .              *
* Use in parseOperandStringFormatToVoidPtr(char *s) but do not define. *
***********************************************************************/
Operand *alloc_Operand(OPERAND_VALUE_TYPE value);

/*
* Recognizes which chars can be part of an Operand string format.
* The Operand string format must not include any char that is also the first char of an Operator symbol, use checkOperatorAndOperandCharsDefinitions() to be sure of this (can only be used after operatorsSymbolTree has been created with create_SymbolTree() ).
* The Operand string format must not include any reserved char.
* Reserverd chars: '\0'  '('  ')'  ',' 
*/
extern int isOperandChar(char c);

/*************************************************************************************************************************************************************
* Takes as input the string format of an operand and returns pointer to newly allocated operand                                                              *
* ,in case of error returns NULL .                                                                                                                           *
* Should look like this:                                                                                                                                     *
*                                                                                                                                                            *
void *parseOperandStringFormatToVoidPtr(char *s){                                                                                                            *
    OPERAND_VALUE_TYPE ret;                                                                                                                                  *
    //parse the string to assign a value to ret                                                                                                              *
    if (error) return NULL;                                                                                                                                  *
    return (void*) alloc_Operand(ret);                                                                                                                       *
}                                                                                                                                                            *
*                                                                                                                                                            *
* eg if OPERAND_VALUE_TYPE is float the string format will be "%f" and sscanf() will be used to parse inputs, which will look like "0.1","31.07","4.04",etc  *
*************************************************************************************************************************************************************/
extern void *parseOperandStringFormatToVoidPtr(char *s);

extern void release_OperandValue(OPERAND_VALUE_TYPE value);

extern void print_OperandValue(OPERAND_VALUE_TYPE val);

/* Operators */
typedef enum{PREFIX,INFIX,POSTFIX} Fix;
typedef struct Operator{
	const char* symbol;
	const int arity;
	const Fix fix;
	const int precedence;
	OPERAND_VALUE_TYPE (*function)(OPERAND_VALUE_TYPE[]); /* Each function will expect an args array size according to the operator's arity */
} Operator;
/*****************************************************************************
* Define your operators and then define the operators array with them in it. * 
* Reserved chars '\0'  '('  ')'  ','                                         *
* Supported arities: any for PREFIX and POSTFIX, 2 for INFIX                 *
*****************************************************************************/
//Look in examples to see what the definition should look like
extern const Operator *operators[];
/************************************************************
* Should look like                                          *
int operatorsCount = sizeof(operators) / sizeof(Operator*); *
************************************************************/
extern int operatorsCount;

/* Symbol recognition decision tree */
typedef struct SymbolTreeNode{
    char c;
    const Operator *op;
    int branchCount;
    struct SymbolTreeNode **branches;
} SymbolTreeNode;

/***************************************
* IMPORTANT!                           *
* Do not define this one.              *
* It is defined in ExpressionToken.c . *
***************************************/
extern SymbolTreeNode *operatorsSymbolTree;
/***********************************************************************************
* IMPORTANT!                                                                       *
* Use at the beginning of main():                                                  *
operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work *
***********************************************************************************/
SymbolTreeNode *create_SymbolTree();
