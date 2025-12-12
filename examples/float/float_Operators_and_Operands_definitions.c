#include "../../Operators_and_Operands_definitions.h"
#include <math.h>

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

float zeroFunc(){
	return 0.0;
}

float oneFunc(){
	return 1.0;
}

float trisumFunc(OPERAND_VALUE_TYPE args[3]){
	return args[0]+args[1]+args[2];
}


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
int operatorsCount = sizeof(operators) / sizeof(Operator*);


int isOperandChar(char c){
	//IF I EVER IMPLEMENT SUPPORT FOR OPERATORS AND OPERANDS THAT SHARE CHARS (overlap)
	//THEN THIS FUNCTION SHOULD RETURN TRUE ONLY IF c ISN'T ALSO AN OPERATOR CHAR

	//for floats
	if (c=='.'|| c>='0'&&c<='9' ) return 1;
	return 0;
}

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

void print_OperandValue(OPERAND_VALUE_TYPE val){
	//for floats
	printf("%f",val);
	return;
}

