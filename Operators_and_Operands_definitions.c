#include "Operators_and_Operands_definitions.h"

int isWhiteSpace(char c){
	if (c==' '||c=='\t'||c=='\n'||c=='\r') return 1;
	return 0;
}

int isReservedChar(char c){
	if (c=='\0'||c=='('||c==')'||c==',') return 1;
	return 0;
}

//#define OPERAND_VALUE_TYPE float

int isOperatorChar(char c){ //could use the symbol tree or a table, blah blah blah
	for (int i=0;i<operatorsCount;i++){
		for (int j=0;operators[i]->symbol[j]!='\0';j++){
			if (c==operators[i]->symbol[j]) return 1;
		}
	}
	return 0;
}

void print_avalaible_Operators(){
	printf("Available operations:\n");
	for (int i=0;i<operatorsCount;i++){
		printf(" Symbol: \"\033[36m%s\033[0m\", Arity: \033[36m%d\033[0m, Fix: \033[36m%s\033[0m, Precedence: \033[36m%d\033[0m\n"
				,operators[i]->symbol
				,operators[i]->arity
				,operators[i]->fix==PREFIX?"PREFIX":(
						operators[i]->fix==INFIX?"INFIX":(
							operators[i]->fix==POSTFIX?"POSTFIX":"ERROR"
						)
					)
				,operators[i]->precedence
			);
	}
	return;
}

Operand *alloc_Operand(OPERAND_VALUE_TYPE value){
	Operand *o = malloc(sizeof(Operand));
	//if (!o) ...
	o->value = value;
	return o;
}

void free_Operand(Operand *operand){
	release_OperandValue(operand->value);
	free(operand);
	return;
}

void print_Operand(Operand *o){
	if (!o){
		printf("print_Operand(NULL)");
		return;
	}
	print_OperandValue(o->value);
	return;
}

/* IMPORTANT
* Use at the beginning of main():
*	checkCompatibilityOperatorAndOperandChars();
* to check that:
* 	1) Operator symbols don't include one of the reserved chars '\0', '(', ')', ','
* 	2) Operand string format doesn't include one the reserved chars '\0', '(', ')', ','
* 	3) No char is both in an Operator symbol and in the Operand string format
*   4) No two Operators have the same symbol (gets checked in create_SymbolTree too)
*/
int checkCompatibilityOperatorAndOperandChars(/* ExpressionContext *context */){

	for (int i=0;i<operatorsCount;i++){
		if (operators[i]->fix==INFIX && operators[i]->arity!=2){
			printf("\033[31mERROR\033[0m in Operator \"\033[36m%s\033[0m\"'s definition: an INFIX operator must have arity=2\n",operators[i]->symbol);
			return 0;
		}

		for (int j=i+1;j<operatorsCount;j++){
			if (!strcmp(operators[i]->symbol,operators[j]->symbol)){
				printf("\033[31mERROR\033[0m in Operator symbols definition: operators number %d and %d have the same symbol \"\033[36m%s\033[0m\"\n",i,j,operators[i]->symbol);
				return 0;
			}
		}
	}

	if (
		isOperatorChar('\0') || isOperatorChar('(') || isOperatorChar(')') || isOperatorChar(',')
	){
		printf("\033[31mERROR\033[0m in Operator symbols definition: a reserved char ('\\0','(',')',',') is recognized as an Operator char by isOperatorChar()\n");
		return 0;
	}
	if (
		isOperandChar('\0') || isOperandChar('(') || isOperandChar(')') || isOperandChar(',')
	){
		printf("\033[31mERROR\033[0m in Operand string format definition: a reserved char ('\\0','(',')',',') is recognized as an Operand char by isOperandChar()\n");
		return 0;
	}
	
	for (int i=0;i<256;i++){
		if (
			(char)i != '\0' && (char)i != '(' &&(char)i != ')' && (char)i != ','
			&& isOperatorChar((char)i) && isOperandChar((char)i)
		){
			printf("\033[31mERROR\033[0m due to incompatible Operator symbols and Operand string format definitions: char %c is recognized both as an Operator char by isOperatorChar() and as an Operand char by isOperandChar()\n",(char)i);
			return 0;
		}
	}

	printf("\033[32mCORRECT\033[0m Operator symbols and Operand string format definitions\n");
	return 1;
}











