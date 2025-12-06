#include "ExpressionTree_V2_Evaluation.c"

int main(){
	
	if (!checkCompatibilityOperatorAndOperandChars()){
		return 1;
	}

	print_avalaible_Operators();

	//char input[] = "11";
	//char input[] = "2+2";
	//char input[] = "sqrt( 0.1/4^.2 +5.7)"; //Result = 2.403286
	//char input[] = " 4 + 3^2";
	//char input[] = "sqrt (0.1) /4^.2 +5.7"; //Result = 5.939656
	//char input[] = "sqrt 0.1 /4^.2 +5.7"; //Result = 5.939656
	//char input[] = "sqrt( 0.1^4/.2 +5.7)"; //Result = 2.387572

	//char input[] = "trisum 1(2, 3)"; //Result = 6
	char input[] = "Z+I+I"; //Result = 2

	printf("Input: \033[7m%s\033[0m\n",input);
	
	ExpressionString es = create_ExpressionString(input);
	/*
	printf("ExpressionString: %s\n",es.str);
	
	ExpressionToken tok = {NULLTERM,NULL};
	printf("Parsed tokens: ");
	while ( (tok = get_next_ExpressionToken_from_ExpressionString(&es)).type!=NULLTERM ){
		print_ExpressionToken(tok);
		printf("_");
	}
	es.index = 0;
	printf("\n");
	*/

	ExpressionToken_Vector *tokenVec = create_ExpressionToken_Vector_from_ExpressionString(&es);
	printf("Token Vector: ");
	
	for (int i=0;i<tokenVec->count;i++){
		print_ExpressionToken(tokenVec->array[i]);
		printf("_");
	}
	printf("\n");
	tokenVec->index = 0;

	ExpressionTreeNode *tree = create_ExpressionTree_from_ExpressionToken_Vector(tokenVec);

	print_ExpressionTree(tree);
	printf("\n");

	OperandVec *resVec = evaluate_ExpressionTree(tree);
	float res = resVec->values[0] ;

	printf("Result = %f\n",res);

	/*
	float a = 2.0f;
	float b = 2.0f;
	float args[2]; args[0]=a; args[1]=b;
	printf("%f %s %f = %f \n",a,sumOp.symbol,b, sumOp.function(args) );   //( (float(*)(float,float))sumOp.function )(a,b) );
	*/

	return 0;
}





