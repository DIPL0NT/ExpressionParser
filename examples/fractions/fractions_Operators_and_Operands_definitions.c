#include "../../Operators_and_Operands_definitions.h"

int isOperandChar(char c){
    if (c=='/'||('0'<=c&&c<='9')){
        return 1;
    }
    return 0;
}

void *parseOperandStringFormatToVoidPtr(char *s){
    OPERAND_VALUE_TYPE ret;
    int converted = sscanf(s,"%d/%d",&ret.num,&ret.den);
    if (converted!=2) return NULL;
    return (void*) alloc_Operand(ret);
} 

void release_OperandValue(OPERAND_VALUE_TYPE value){
    return;
}

void print_OperandValue(OPERAND_VALUE_TYPE val){
    printf("%d/%d",val.num,val.den);
    return;
}

void sprint_OperandValue(char *s,OPERAND_VALUE_TYPE val){
    sprintf(s,"%d/%d",val.num,val.den);
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

