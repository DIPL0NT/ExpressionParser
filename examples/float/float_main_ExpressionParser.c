#include "float_Operators_and_Operands_definitions.c"


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

	ExpressionTestCase tests[] ={
		{"11"						,"11.000000"},
		{"2+2"						,"4.000000"},
		{" 4 + 3^2"					,"13.000000"},
		{"3^2+4"					,"13.000000"},
		{"( 0.1/4^.2 +5.7)"			,"5.775785"},
		{"sqrt( 0.1/4^.2 +5.7)"		,"2.403286"},
		{"5+ sqrt4"					,"7.000000"},
		{"5+ sqrt(4)"				,"7.000000"},
		{"sqrt (0.1) /4^.2 +5.7"	,"5.939656"},
		{"sqrt 0.1 /4^.2 +5.7"		,"5.939656"},
		{"sqrt( 0.1^4/.2 +5.7)"		,"2.387572"},
		{"Z + I + I"				,"2.000000"},
		{"Z+I+I"					,"2.000000"},
		{"trisum 1 2 3"				,"6.000000"},
		{"trisum(1,2,3)"			,"6.000000"},
		{"trisum(1,2,3,4)"			,"evaluate_ExpressionTree(NULL)"}, //causes leak
		{"trisum((1,2,3))"			,"6.000000"},
		{"trisum((1,2),3)"			,"6.000000"},
		{"trisum(1,(2,3))"			,"6.000000"},
		{"trisum(1,(2),3)"			,"6.000000"},
		{"trisum((1,2,3,4))"		,"evaluate_ExpressionTree(NULL)"}, //causes leak
		{"trisum((1,2,3),4)"		,"evaluate_ExpressionTree(NULL)"}, //causes leak
		{"trisum(1,(2,3,4))"		,"evaluate_ExpressionTree(NULL)"}, //causes leak
		{"trisum 1(2, 3)"			,"evaluate_ExpressionTree(NULL)"},
		{"trisum (1,2)3"			,"evaluate_ExpressionTree(NULL)"},
		{"trisum 1 (2, 3)"			,"6.000000"},
		{"trisum (1,2) 3"			,"6.000000"},
		{"qetrasum(1,(2,3,4))"		,"10.000000"},
		{"qetrasum(1,2,(3,4))"		,"10.000000"},
		{"qetrasum(1,(2,3),4)"		,"10.000000"},
		{"qetrasum((1,2),(3,4))"	,"10.000000"},
		{"2+2","3.000000"}
	};
	int n = sizeof(tests) / sizeof(ExpressionTestCase) ;
	runExpressionTests(tests,n);
	
	char input[] = "sqrt( 0.1/4^.2 +5.7)";
	//Input acquisition
	printf("Input: \033[7m%s\033[0m\n",input);
	ExpressionString es = create_ExpressionString(input);
	printf("ExpressionString: %s\n",es.str);
	ExpressionToken_Vector *tokenVec = NULL;
	tokenVec = create_ExpressionToken_Vector_from_ExpressionString(operatorsSymbolTree,&es);
	printf("Token Vector: "); print_ExpressionTokenVector(tokenVec);
	//Create ExpressionTree
	ExpressionTreeNode *tree = create_ExpressionTree_from_ExpressionToken_Vector(tokenVec);
	print_ExpressionTree(tree);
	printf("\n");
	//Evaluation
	OperandVec_Wrapper res_wrp = {NULL,NULL};
	res_wrp = evaluate_ExpressionTree(tree);
	if (!is_OperandVec_Wrapper_NULL(res_wrp)){
		float res = VoidPtr_to_Float(res_wrp.vec->values[0]) ;
		printf("Result\033[7m = %f\033[0m\n",res);
	}
	else{
		//
	}
	//CleanUp
	free(es.str);
	if (tokenVec) free_ExpressionToken_Vector(tokenVec);
	if (!is_OperandVec_Wrapper_NULL(res_wrp)){
		release_OperandVec_Wrapper_values(res_wrp);
		free_OperandVec_Wrapper(res_wrp);
	}

	//SECOND TIME
	printf("Second Time\n");
	print_ExpressionTree(tree);
	printf("\n");
	//Evaluation
	res_wrp = (OperandVec_Wrapper){NULL,NULL};
	res_wrp = evaluate_ExpressionTree(tree);
	if (!is_OperandVec_Wrapper_NULL(res_wrp)){
		float res = VoidPtr_to_Float(res_wrp.vec->values[0]) ;
		printf("Result\033[7m = %f\033[0m\n",res);
	}
	else{
		//
	}

	//CleanUp
	if (!is_OperandVec_Wrapper_NULL(res_wrp)){
		release_OperandVec_Wrapper_values(res_wrp);
		free_OperandVec_Wrapper(res_wrp);
	}

	if (tree) free_ExpressionTreeNode(tree);
	
	if (operatorsSymbolTree) free_SymbolTreeNode(operatorsSymbolTree);

	return 0;
}


