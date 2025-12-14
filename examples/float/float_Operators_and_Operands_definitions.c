#include "../../ExpressionParser.c"
#include <math.h>

typedef union F_VP{
	float f;
	void* vp;
} F_VP;

#define Float_to_VoidPtr(f) ( ((F_VP)(f)).vp )
#define VoidPtr_to_Float(p) ( ((F_VP)(p)).f )



OPERAND_VALUE_TYPE sumFunc(OPERAND_VALUE_TYPE args[2]){
	return Float_to_VoidPtr( VoidPtr_to_Float(args[0])+VoidPtr_to_Float(args[1]) ) ;
}

OPERAND_VALUE_TYPE subFunc(OPERAND_VALUE_TYPE args[2]){
	return Float_to_VoidPtr( VoidPtr_to_Float(args[0])-VoidPtr_to_Float(args[1]) );
}

OPERAND_VALUE_TYPE multFunc(OPERAND_VALUE_TYPE args[2]){
	return Float_to_VoidPtr( VoidPtr_to_Float(args[0])*VoidPtr_to_Float(args[1]) );
}

OPERAND_VALUE_TYPE divFunc(OPERAND_VALUE_TYPE args[2]){
	return Float_to_VoidPtr( VoidPtr_to_Float(args[0])/VoidPtr_to_Float(args[1]) );
}

OPERAND_VALUE_TYPE sqrtFunc(OPERAND_VALUE_TYPE args[1]){
	float f = sqrt(VoidPtr_to_Float(args[0]));
	return Float_to_VoidPtr( f );
}

OPERAND_VALUE_TYPE powFunc(OPERAND_VALUE_TYPE args[2]){
	float f = pow(VoidPtr_to_Float(args[0]),VoidPtr_to_Float(args[1]));
	return Float_to_VoidPtr( f );
}

OPERAND_VALUE_TYPE zeroFunc(){
	return Float_to_VoidPtr(0.0f);
}

OPERAND_VALUE_TYPE oneFunc(){
	return Float_to_VoidPtr(1.0f);
}

OPERAND_VALUE_TYPE trisumFunc(OPERAND_VALUE_TYPE args[3]){
	float f = VoidPtr_to_Float(args[0])+VoidPtr_to_Float(args[1])+VoidPtr_to_Float(args[2]);
	return Float_to_VoidPtr( f );
}

OPERAND_VALUE_TYPE tetrasumFunc(OPERAND_VALUE_TYPE args[4]){
	float f = VoidPtr_to_Float(args[0])+VoidPtr_to_Float(args[1])+VoidPtr_to_Float(args[2])+VoidPtr_to_Float(args[3]);
	return Float_to_VoidPtr( f );
}


const Operator sumOp  	  = {"+"	 	 ,2     ,INFIX  ,0          , sumFunc		};
const Operator subOp  	  = {"-"	 	 ,2     ,INFIX  ,0          , subFunc		};
const Operator multOp 	  = {"*"	 	 ,2     ,INFIX  ,1          , multFunc		};
const Operator divOp  	  = {"/"     	 ,2     ,INFIX  ,1          , divFunc		};
const Operator sqrtOp 	  = {"sqrt"	 	 ,1     ,PREFIX ,2          , sqrtFunc		};
const Operator powOp  	  = {"^"	 	 ,2     ,INFIX  ,2          , powFunc		};
const Operator zeroOp 	  = {"Z"	 	 ,0     ,PREFIX ,3          , zeroFunc		};
const Operator oneOp  	  = {"I"	 	 ,0     ,PREFIX ,3          , oneFunc		};
const Operator trisumOp   = {"trisum"	 ,3     ,PREFIX ,2          , trisumFunc	};
const Operator tetrasumOp = {"qetrasum"	 ,4     ,PREFIX ,2          , tetrasumFunc	};

const Operator *operators[] = {&sumOp,&subOp,&multOp,&divOp,&sqrtOp,&powOp,&zeroOp,&oneOp,&trisumOp,&tetrasumOp};
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
	float f;
	if (0==sscanf(s,"%f",&f)){
		printf("\033[31mERROR\033[0m in parseOperandStringFormatToVoidPtr() : couldn't parse %s\n",s);
		return NULL;
	}
	return (void*) alloc_Operand( Float_to_VoidPtr(f) );
}

void release_OperandValue(OPERAND_VALUE_TYPE value){
	//for floats do nothing
	return;
}

void print_OperandValue(OPERAND_VALUE_TYPE val){
	//for floats
	printf("%f", VoidPtr_to_Float(val));
	return;
}

void sprint_OperandValue(char *s,OPERAND_VALUE_TYPE val){
	sprintf(s,"%f",VoidPtr_to_Float(val));
	return;
}


