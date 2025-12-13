#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/****************************************************************************************************************
 Include ExpressionParser.c in a file where you will define everything that is marked as TODEFINE in this file.
 The functions that actually implement the operators must receive and array of void* and return a void*
 Before using the parser must set up with:
operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work
checkOperatorAndOperandCharsDefinitions(); //to check there's no errors in the definitions
 Run tests with:
runExpressionTests(your_tests,your_testsCount);
****************************************************************************************************************/

#define OPERAND_VALUE_TYPE void*

typedef struct Operand{
	OPERAND_VALUE_TYPE value;
} Operand;

/***********************************************************************
 IMPORTANT!
 Needed for parseOperandStringFormatToVoidPtr(char *s) .
 Use in parseOperandStringFormatToVoidPtr(char *s) but do not define.
***********************************************************************/
Operand *alloc_Operand(OPERAND_VALUE_TYPE value);

/*
 Recognizes which chars can be part of an Operand string format.
 The Operand string format must not include any char that is also the first char of an Operator symbol, use checkOperatorAndOperandCharsDefinitions() to be sure of this (can only be used after operatorsSymbolTree has been created with create_SymbolTree() ).
 The Operand string format must not include any reserved char.
 Reserverd chars: '\0'  '('  ')'  ',' 
*/
/*TODEFINE*/ int isOperandChar(char c);

/*************************************************************************************************************************************************************
 Takes as input the string format of an operand and returns pointer to newly allocated operand
 ,in case of error returns NULL .
 Should look like this:

/examples/float/float_Operators_and_Operands_definitions.c:
void *parseOperandStringFormatToVoidPtr(char *s){
	float f;
	if (1!=sscanf(s,"%f",&f)){
		printf("\033[31mERROR\033[0m in parseOperandStringFormatToVoidPtr() : couldn't parse %s\n",s);
		return NULL;
	}
	return (void*) alloc_Operand( Float_to_VoidPtr(f) );
}

/examples/fractions/fractions_Operators_and_Operands_definitions.c:
void *parseOperandStringFormatToVoidPtr(char *s){
    int num; int den;
    if (2!=sscanf(s,"%d/%d",&num,&den)){
        return NULL;
    }
    fraction *f = alloc_fraction(num,den);
    return (void*) alloc_Operand(f);
}

*************************************************************************************************************************************************************/
/*TODEFINE*/ void *parseOperandStringFormatToVoidPtr(char *s);

/*TODEFINE*/ void release_OperandValue(OPERAND_VALUE_TYPE value);

/*TODEFINE*/ void print_OperandValue(OPERAND_VALUE_TYPE val);

/*TODEFINE*/ void sprint_OperandValue(char *s,OPERAND_VALUE_TYPE val);

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
 Define your operators and then define the operators array with them in it.
 Reserved chars '\0'  '('  ')'  ','
 Supported arities: any for PREFIX and POSTFIX, 2 for INFIX
*****************************************************************************/
//Look in examples to see what the definition should look like
/*TODEFINE*/ const Operator *operators[];
/************************************************************
 Should look like
int operatorsCount = sizeof(operators) / sizeof(Operator*);
************************************************************/
/*TODEFINE*/ int operatorsCount;

/* Symbol recognition decision tree */
typedef struct SymbolTreeNode{
    char c;
    const Operator *op;
    int branchCount;
    struct SymbolTreeNode **branches;
} SymbolTreeNode;

/***************************************
 Do not define.
***************************************/
SymbolTreeNode *operatorsSymbolTree;
/***********************************************************************************
 IMPORTANT!
 Do not define.
 Use at the beginning of main():
operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work
***********************************************************************************/
SymbolTreeNode *create_SymbolTree();



