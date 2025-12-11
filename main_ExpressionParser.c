#include "ExpressionParser.c"

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

const Operator sumOp  = {"+"	 ,2     ,INFIX  ,0          , sumFunc	};
const Operator subOp  = {"-"	 ,2     ,INFIX  ,0          , subFunc	};
const Operator multOp = {"*"	 ,2     ,INFIX  ,1          , multFunc	};
const Operator divOp  = {"/"     ,2     ,INFIX  ,1          , divFunc	};
const Operator sqrtOp = {"sqrt"	 ,1     ,PREFIX ,2          , sqrtFunc	};
const Operator powOp  = {"^"	 ,2     ,INFIX  ,2          , powFunc	};
const Operator zeroOp = {"Z"	 ,0     ,PREFIX ,3          , zeroFunc	};
const Operator oneOp  = {"I"	 ,0     ,PREFIX ,3          , oneFunc	};
const Operator trisumOp = {"trisum"	 ,3     ,PREFIX ,2          , trisumFunc	};

const Operator *float_operators[] = {&sumOp,&subOp,&multOp,&divOp,&sqrtOp,&powOp,&zeroOp,&oneOp,&trisumOp};
int float_operatorsCount = sizeof(float_operators) / sizeof(Operator*);

Operand *float_alloc_Operand(OPERAND_VALUE_TYPE value){
	Operand *o = malloc(sizeof(Operand));
	//if (!o) ...
	o->value = value;
	return o;
}

//reserverd chars: '\0', '(', ')', ',' and all the operators chars
int float_isOperandChar(char c){
	//IF I EVER IMPLEMENT SUPPORT FOR OPERATORS AND OPERANDS THAT SHARE CHARS (overlap)
	//THEN THIS FUNCTION SHOULD RETURN TRUE ONLY IF c ISN'T ALSO AN OPERATOR CHAR

	//for floats
	if (c=='.'|| c>='0'&&c<='9' ) return 1;
	return 0;
}

//takes as input the string format of an operand and returns pointer to newly allocated operand
//in case of error returns NULL
//eg if operands are float the input will be "%f" and sscanf() will be used to parse
void *float_parseOperandStringFormatToVoidPtr(char *s){
	//for floats
	OPERAND_VALUE_TYPE f;
	if (0==sscanf(s,"%f",&f)){
		printf("\033[31mERROR\033[0m in parseOperandStringFormatToVoidPtr() : couldn't parse %s\n",s);
		return NULL;
	}
	return (void*) float_alloc_Operand(f);
}

void float_release_OperandValue(OPERAND_VALUE_TYPE value){
	//for floats do nothing
	return;
}

void float_free_Operand(Operand *operand){
	float_release_OperandValue(operand->value);
	free(operand);
	return;
}

void float_print_OperandValue(OPERAND_VALUE_TYPE val){
	//for floats
	printf("%f",val);
	return;
}

void float_print_Operand(Operand *o){
	if (!o){
		printf("print_Operand(NULL)");
		return;
	}
	float_print_OperandValue(o->value);
	return;
}


testCase float_tests[] ={
	{"11"						,"11.000000"},
	{"2+2"						,"4.000000"},
	{"sqrt( 0.1/4^.2 +5.7)"		,"2.403286"},
	{"5+ sqrt4"					,"7.000000"},
	{" 4 + 3^2"					,"13.000000"},
	{"sqrt (0.1) /4^.2 +5.7"	,"5.939656"},
	{"sqrt 0.1 /4^.2 +5.7"		,"5.939656"},
	{"sqrt( 0.1^4/.2 +5.7)"		,"2.387572"},
	{"trisum 1(2, 3)"			,"6.000000"},
	{"Z + I + I"				,"2.000000"},
	//{"Z+I+I"				,"2.000000"},
};
int float_testsCount = sizeof(float_tests) / sizeof(testCase) ;

int main(){

	ExpressionContext *float_context = alloc_ExpressionContext(
											float_operators,float_operatorsCount,
											float_alloc_Operand,float_release_OperandValue,float_free_Operand,
											float_isOperandChar,float_parseOperandStringFormatToVoidPtr,
											float_print_OperandValue,float_print_Operand
										);
	
	if (0 ){//!checkCompatibilityOperatorAndOperandChars()){
		return 1;
	}

	runTests(float_context,float_tests,float_testsCount);

	//print_avalaible_Operators();

	/*
	char input[] = "11";
	char input[] = "2+2";
	char input[] = "sqrt( 0.1/4^.2 +5.7)"; //Result = 2.403286
	char input[] = "5+ sqrt4";
	char input[] = " 4 + 3^2";
	char input[] = "sqrt (0.1) /4^.2 +5.7"; //Result = 5.939656
	char input[] = "sqrt 0.1 /4^.2 +5.7"; //Result = 5.939656
	char input[] = "sqrt( 0.1^4/.2 +5.7)"; //Result = 2.387572
	char input[] = "trisum 1(2, 3)"; //Result = 6
	*/
	
	char input[] = "2+sqrt(2)"; 
	//char input[] = "Z+I+I"; //Result = 2

	/*Input acquisition*/
	printf("Input: \033[7m%s\033[0m\n",input);
	ExpressionString es = create_ExpressionString(input);
	printf("ExpressionString: %s\n",es.str);
	ExpressionToken tok = {NULLTERM,NULL};
	printf("Parsed tokens: ");
	while ( (tok = get_next_ExpressionToken_from_ExpressionString(float_context,&es)).type!=NULLTERM ){
		print_ExpressionToken(tok);
		printf("_");
	}
	es.index = 0;
	printf("\n");
	
	ExpressionToken_Vector *tokenVec = NULL;
	tokenVec = create_ExpressionToken_Vector_from_ExpressionString(float_context,&es);
	printf("Token Vector: ");
	for (int i=0;i<tokenVec->count;i++){
		print_ExpressionToken(tokenVec->array[i]);
		printf("_");
	}
	printf("\n");
	tokenVec->index = 0;
	/*Create ExpressionTree*/
	ExpressionTreeNode *tree = create_ExpressionTree_from_ExpressionToken_Vector(tokenVec);
	print_ExpressionTree(tree);
	printf("\n");
	/*Evaluation*/
	OperandVec *resVec = NULL;
	resVec = evaluate_ExpressionTree(tree);
	if (resVec){
		float res = resVec->values[0] ;
		printf("Result = %f\n",res);
	}
	else{
		//
	}
	
	/*CleanUp*/
	free(es.str);
	if (tokenVec) free_ExpressionToken_Vector(tokenVec);
	if (resVec) free_OperandVec(resVec);
	if (tree) free_ExpressionTreeNode(tree);
	if (float_context) free_ExpressionContext(float_context);

	return 0;
}



