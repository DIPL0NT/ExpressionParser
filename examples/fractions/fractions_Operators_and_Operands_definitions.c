#include "../../ExpressionParser.c"
#include "fractions_implementation/fractions.h"

#define Fraction_to_VoidPtr(f) ( (void*)(f) )
#define VoidPtr_to_Fraction(p) ( (fraction*)(p) )

int isOperandChar(char c){
    if (c=='/'||('0'<=c&&c<='9')){
        return 1;
    }
    return 0;
}

void *parseOperandStringFormatToVoidPtr(char *s){
    int num; int den;
    int converted = sscanf(s,"%d/%d",&num,&den);
    if (converted!=2) return NULL;
    fraction *f = alloc_fraction(num,den);
    return (void*) alloc_Operand(f);
} 

void release_OperandValue(OPERAND_VALUE_TYPE value){
    //if (!value) ...
    free_fraction(value);
    return;
}

void print_OperandValue(OPERAND_VALUE_TYPE val){
    //if (!val) ...
    printf("%d/%d",VoidPtr_to_Fraction(val)->num,VoidPtr_to_Fraction(val)->den);
    return;
}

void sprint_OperandValue(char *s,OPERAND_VALUE_TYPE val){
    //if (!s) ...
    //if (!val) ...
    sprintf(s,"%d/%d",VoidPtr_to_Fraction(val)->num,VoidPtr_to_Fraction(val)->den);
    return;
}

OPERAND_VALUE_TYPE sumFunc(OPERAND_VALUE_TYPE args[2]){
	return sum_fractions(args[0],args[1]);
}

OPERAND_VALUE_TYPE subFunc(OPERAND_VALUE_TYPE args[2]){
	return sub_fractions(args[0],args[1]);
}

OPERAND_VALUE_TYPE multFunc(OPERAND_VALUE_TYPE args[2]){
	return mult_fractions(args[0],args[1]);
}

OPERAND_VALUE_TYPE divFunc(OPERAND_VALUE_TYPE args[2]){
	return div_fractions(args[0],args[1]);
}

const Operator sumOp  = {"+"	 ,2     ,INFIX  ,0          , sumFunc	};
const Operator subOp  = {"-"	 ,2     ,INFIX  ,0          , subFunc	};
const Operator multOp = {"*"	 ,2     ,INFIX  ,1          , multFunc	};
const Operator divOp  = {":"	 ,2     ,INFIX  ,1          , divFunc	};

const Operator *operators[] = {&sumOp,&subOp,&multOp,&divOp};
int operatorsCount = sizeof(operators) / sizeof(Operator*);

