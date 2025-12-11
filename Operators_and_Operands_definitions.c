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

/*
OPERAND_VALUE_TYPE sumFunc(OPERAND_VALUE_TYPE args[2]){
	return args[0]+args[1];
}

OPERAND_VALUE_TYPE subFunc(OPERAND_VALUE_TYPE args[2]){
	return args[0]-args[1];
}

OPERAND_VALUE_TYPE multFunc(OPERAND_VALUE_TYPE args[2]){
	return args[0]*args[1];
}

OPERAND_VALUE_TYPE divFunc(OPERAND_VALUE_TYPE args[2]){
	return args[0]/args[1];
}

OPERAND_VALUE_TYPE sqrtFunc(OPERAND_VALUE_TYPE args[1]){
	return sqrt(args[0]);
}

OPERAND_VALUE_TYPE powFunc(OPERAND_VALUE_TYPE args[2]){
	return pow(args[0],args[1]);
}

OPERAND_VALUE_TYPE zeroFunc(){
	return 0.0;
}

OPERAND_VALUE_TYPE oneFunc(){
	return 1.0;
}

OPERAND_VALUE_TYPE trisumFunc(OPERAND_VALUE_TYPE args[3]){
	return args[0]+args[1]+args[2];
}
*/

typedef enum{PREFIX,INFIX,POSTFIX} Fix;
typedef struct Operator{
	const char* symbol;
	const int arity; //0, 1, 2
	const Fix fix; //0: prefix, 1: infix
	const int precedence;
	OPERAND_VALUE_TYPE (*function)(OPERAND_VALUE_TYPE[]); //each function will expect an args array size according to the operator's arity
} Operator;

//reserved chars '\0', '(', ')', ','
//supported arities: any for PREFIX and POSTFIX, 2 for INFIX
//                       symbol   arity  fix     precedence   function
/*
const Operator sumOp  = {"+"	 ,2     ,INFIX  ,0          , sumFunc	};
const Operator subOp  = {"-"	 ,2     ,INFIX  ,0          , subFunc	};
const Operator multOp = {"*"	 ,2     ,INFIX  ,1          , multFunc	};
const Operator divOp  = {"/"     ,2     ,INFIX  ,1          , divFunc	};
const Operator sqrtOp = {"sqrt"	 ,1     ,PREFIX ,2          , sqrtFunc	};
const Operator powOp  = {"^"	 ,2     ,INFIX  ,2          , powFunc	};
const Operator zeroOp = {"Z"	 ,0     ,PREFIX ,3          , zeroFunc	};
const Operator oneOp  = {"I"	 ,0     ,PREFIX ,3          , oneFunc	};
const Operator trisumOp = {"trisum"	 ,3     ,PREFIX ,2          , trisumFunc	};

const Operator *operators[] = {&sumOp,&subOp,&multOp,&divOp,&sqrtOp,&powOp,&zeroOp,&oneOp,&trisumOp};
int NUMofOPERATORS = sizeof(operators) / sizeof(Operator*);
*/

//int isOperatorChar(char c);

//void print_availableOperators();

//the define below is at the top of the file since it is needed for the operator functions
//#define OPERAND_VALUE_TYPE float
typedef struct Operand{
	OPERAND_VALUE_TYPE value;
} Operand;

/*
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
		printf("\033[31mERROR\033[0m in parseOperandStringFormatToVoidPtr() : couldn't parse %s\n",s);
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
	if (!o){
		printf("print_Operand(NULL)");
		return;
	}
	print_OperandValue(o->value);
	return;
}

*/

/* IMPORTANT
* Use at the beginning of main() to check that:
* 	1) Operator symbols don't include one of the reserved chars '\0', '(', ')', ','
* 	2) Operand string format doesn't include one the reserved chars '\0', '(', ')', ','
* 	3) No char is both in an Operator symbol and in the Operand string format
*/
/*
int checkCompatibilityOperatorAndOperandChars(){

	for (int i=0;i<NUMofOPERATORS;i++){
		if (operators[i]->fix==INFIX && operators[i]->arity!=2){
			printf("\033[31mERROR\033[0m in Operator \"\033[36m%s\033[0m\"'s definition: an INFIX operator must have arity=2\n",operators[i]->symbol);
			return 0;
		}

		for (int j=i+1;j<NUMofOPERATORS;j++){
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
*/



