#include "ExpressionParser.c"



int main(){
	
	if (!checkCompatibilityOperatorAndOperandChars()){
		return 1;
	}

	runTests();

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
	while ( (tok = get_next_ExpressionToken_from_ExpressionString(&es)).type!=NULLTERM ){
		print_ExpressionToken(tok);
		printf("_");
	}
	es.index = 0;
	printf("\n");
	
	ExpressionToken_Vector *tokenVec = NULL;
	tokenVec = create_ExpressionToken_Vector_from_ExpressionString(&es);
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
	

	return 0;
}




