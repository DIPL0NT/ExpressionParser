#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int isWhiteSpace(char c){
	if (c==' '||c=='\t'||c=='\n'||c=='\r') return 1;
	return 0;
}

int isReservedChar(char c){
	if (c=='\0'||c=='('||c==')'||c==',') return 1;
	return 0;
}

#define OPERAND_VALUE_TYPE float

OPERAND_VALUE_TYPE sumFunc(OPERAND_VALUE_TYPE args[2]){
	return args[0]+args[1];
}

float subFunc(OPERAND_VALUE_TYPE args[2]){
	return args[0]-args[1];
}

float multFunc(OPERAND_VALUE_TYPE args[2]){
	return args[0]*args[1];
}

float divFunc(OPERAND_VALUE_TYPE args[2]){
	return args[0]/args[1];
}

float sqrtFunc(OPERAND_VALUE_TYPE args[1]){
	return sqrt(args[0]);
}

float powFunc(OPERAND_VALUE_TYPE args[2]){
	return pow(args[0],args[1]);
}

typedef enum{PREFIX,INFIX,POSTFIX} Fix;
typedef struct Operator{
	const char* symbol;
	const int arity; //0, 1, 2
	const Fix fix; //0: prefix, 1: infix
	const int precedence;
	OPERAND_VALUE_TYPE (*function)(OPERAND_VALUE_TYPE[]); //each function will expect an args array size according to the operator's arity
} Operator;

//reserved chars '\0', '(', ')', ','
//supported arities: 0, 1, 2
//                       symbol   arity  fix     precedence   function
const Operator sumOp  = {"+"	 ,2     ,INFIX  ,0          , sumFunc	};
const Operator subOp  = {"-"	 ,2     ,INFIX  ,0          , subFunc	};
const Operator multOp = {"*"	 ,2     ,INFIX  ,1          , multFunc	};
const Operator divOp  = {"/"     ,2     ,INFIX  ,1          , divFunc	};
const Operator sqrtOp = {"sqrt"	 ,1     ,PREFIX ,2          , sqrtFunc	};
const Operator powOp  = {"**"	 ,2     ,INFIX  ,2          , powFunc	};
#define NUMofOPERATORS 6
const Operator *operators[NUMofOPERATORS] = {&sumOp,&subOp,&multOp,&divOp,&sqrtOp,&powOp};

int isOperatorChar(char c){ //could be implemented as table, blah blah blah
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

//the define below is at the top of the file since it is needed for the operator functions
//#define OPERAND_VALUE_TYPE float
typedef struct Operand{
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
	//IF I EVER IMPLEMENT SUPPORT FOR OPERATORS AND OPERANDS THAT SHARE CHARS (overlap)
	//THEN THIS FUNCTION SHOULD RETURN TRUE ONLY IF c ISN'T ALSO AN OPERATOR CHAR

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
	//for floats do nothing
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





