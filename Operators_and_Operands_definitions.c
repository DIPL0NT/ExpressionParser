#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>

int isWhiteSpace(char c){
	if (c==' '||c=='\t'||c=='\n'||c=='\r') return 1;
	return 0;
}

typedef enum{PREFIX,INFIX,POSTFIX} Fix;

typedef struct Operator{
	const char* symbol;
	const int arity; //0, 1, 2
	const Fix fix; //0: prefix, 1: infix
	const int precedence;
	//function pointer?
} Operator;

//reserved chars '\0', '(', ')', ','
//supported arities: 0, 1, 2

const Operator sumOp  = {"+"		,2,INFIX    ,0};
const Operator subOp  = {"-"		,2,INFIX    ,0};
const Operator multOp = {"*"		,2,INFIX    ,1};
const Operator divOp  = {"/"		,2,INFIX    ,1};
const Operator sqrtOp = {"sqrt"		,1,PREFIX   ,2};
const Operator powOp  = {"**"		,2,INFIX    ,2};
#define NUMofOPERATORS 6
Operator *operators[NUMofOPERATORS] = {&sumOp,&subOp,&multOp,&divOp,&sqrtOp,&powOp};

int isOperatorChar(char c){
	for (int i=0;i<NUMofOPERATORS;i++){
		for (int j=0;operators[i]->symbol[j]!='\0';j++){
			if (c==operators[i]->symbol[j]) return 1;
		}
	}
	return 0;
}

void print_avalaible_Operators(){
	printf("Available operations:\n");
	for (int i=0;i<NUMofOPERATORS;i++){
		printf(" Symbol: \"%s\", Arity: %d, Fix: %s, Precedence: %d\n"
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

#define OPERAND_VALUE_TYPE float
typedef struct Operand{
	//type or smth?
	OPERAND_VALUE_TYPE value;
} Operand;

Operand *alloc_Operand(OPERAND_VALUE_TYPE value){
	Operand *o = malloc(sizeof(Operand));
	//if (!o) ...
	o->value = value;
	return o;
}

//reserverd chars: '\0', '(', ')', ',' and all the operators chars
int isOperandChar(char c){
	//for floats
	if (c=='.'|| c>='0'&&c<='9' ) return 1;
	return 0;
}

//takes as input the string format of an operand and returns pointer to newly allocated operand
//in case of error returns NULL
//eg if operands are float the input will be "%f" and sscanf() will be used to parse
void *parseOperandStringFormatToVoidPtr(char *s){
	//for floats
	OPERAND_VALUE_TYPE f;
	if (0==sscanf(s,"%f",&f)){
		printf("ERROR in parseOperandStringFormatToVoidPtr() : couldn't parse %s\n",s);
		return NULL;
	}
	return (void*) alloc_Operand(f);
}

void release_OperandValue(OPERAND_VALUE_TYPE value){
	//do nothing for floats
	return;
}

void free_Operand(Operand *operand){
	release_OperandValue(operand->value);
	free(operand);
	return;
}

void print_OperandValue(OPERAND_VALUE_TYPE val){
	//for floats
	printf("%f",val);
	return;
}

void print_Operand(Operand *o){
	print_OperandValue(o->value);
	return;
}

/*
* Use at the beginning of main() to check that:
* 	1) Operator symbols don't include one of the reserved chars '\0', '(', ')', ','
* 	2) Operand string format doesn't include one the reserved chars '\0', '(', ')', ','
* 	3) No char is both in an Operator symbol and in the Operand string format
*/
int checkCompatibilityOperatorAndOperandChars(){
	if (
		isOperatorChar('\0') || isOperatorChar('(') || isOperatorChar(')') || isOperatorChar(',')
	){
		printf("ERROR in Operator symbols definition: a reserved char ('\\0','(',')',',') is recognized as an Operator char by isOperatorChar()\n");
		return 0;
	}
	if (
		isOperandChar('\0') || isOperandChar('(') || isOperandChar(')') || isOperandChar(',')
	){
		printf("ERROR in Operand string format definition: a reserved char ('\\0','(',')',',') is recognized as an Operand char by isOperandChar()\n");
		return 0;
	}
	
	for (int i=0;i<256;i++){
		if (
			(char)i != '\0' && (char)i != '(' &&(char)i != ')' && (char)i != ','
			&& isOperatorChar((char)i) && isOperandChar((char)i)
		){
			printf("ERROR due to incompatible Operator symbols and Operand string format definitions: char %c is recognized both as an Operator char by isOperatorChar() and as an Operand char by isOperandChar()\n",(char)i);
			return 0;
		}
	}
	printf("CORRECT Operator symbols and Operand string format definitions\n");
	return 1;
}




