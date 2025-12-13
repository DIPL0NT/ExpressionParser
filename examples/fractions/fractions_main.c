#include "fractions_Operators_and_Operands_definitions.c"


int main(int argc,char** argv){

	operatorsSymbolTree = NULL;
	operatorsSymbolTree = create_SymbolTree();
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

	ExpressionTestCase tests[] = {
		{"1/2+1/3"			, "5/6"		},
		{"5/8 + 3/4 - 1/2"	, "7/8"		},
		{"7/12 * 2/3 + 1/4"	, "23/36"	},
		{"3/7 - 4/9 * 2/5"	, "79/315"	},
		{"6/11 : 2/3 + 5/8"	, "127/88"	},
		{"2/3 + 4/5 - 1/6"	, "13/10"	}
	};
	int n = sizeof(tests) / sizeof(ExpressionTestCase) ;

	runExpressionTests(tests,n);

	char input[] = "7/12 * 2/3 + 1/4";
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
	OperandVec_Wrapper res_wrp = (OperandVec_Wrapper){NULL,NULL};
	res_wrp = evaluate_ExpressionTree(tree);
	if (!is_OperandVec_Wrapper_NULL(res_wrp)){
		fraction *res = VoidPtr_to_Fraction(res_wrp.vec->values[0]) ;
		printf("Result\033[7m = ");
		print_fraction(res);
		printf("\033[0m\n");
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
		fraction *res = VoidPtr_to_Fraction(res_wrp.vec->values[0]) ;
		printf("Result\033[7m = ");
		print_fraction(res);
		printf("\033[0m\n");
	}
	else{
		//
	}

	if (tree) free_ExpressionTreeNode(tree);
	
	if (operatorsSymbolTree) free_SymbolTreeNode(operatorsSymbolTree);

	return 0;
}


