#include "ExpressionParser.c"



int main(){
	
	/*
	NECESSARY
	Setting up and checking definition
	*/
	operatorsSymbolTree = NULL;
	operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work
	if (!operatorsSymbolTree){
		return 1;
	}
	else{
		printf("\033[32mCORRECT\033[0m creation of operatorsSymbolTree\n");
	}
	
	if (!checkOperatorAndOperandCharsDefinitions()){
		return 2;
	}

	print_avalaible_Operators();
	runExpressionTests();

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
	
	char input[] = "sqrt( 0.1/4^.2 +5.7)";

	/*Input acquisition*/
	printf("Input: \033[7m%s\033[0m\n",input);
	ExpressionString es = create_ExpressionString(input);
	printf("ExpressionString: %s\n",es.str);
	
	ExpressionToken_Vector *tokenVec = NULL;
	tokenVec = create_ExpressionToken_Vector_from_ExpressionString(operatorsSymbolTree,&es);
	printf("Token Vector: "); print_ExpressionTokenVector(tokenVec);

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
	
	if (operatorsSymbolTree) free_SymbolTreeNode(operatorsSymbolTree);

	return 0;
}
