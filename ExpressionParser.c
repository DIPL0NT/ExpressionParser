#include "ExpressionTree_Evaluation.c"

typedef struct ExpressionTestCase{
	char *input;
	char *output;
} ExpressionTestCase;

void runExpressionTests(ExpressionTestCase *tests,int testsCount){
	
	

	int fail = 0;
	for (int i=0;i<testsCount;i++){
		printf("\n");
		char *input = tests[i].input;
		printf("\033[7m%s\033[0m\n",input);
		ExpressionString es = create_ExpressionString(input);
		ExpressionToken_Vector *tokenVec = create_ExpressionToken_Vector_from_ExpressionString(operatorsSymbolTree,&es);
		print_ExpressionTokenVector(tokenVec);
		ExpressionTreeNode *tree = create_ExpressionTree_from_ExpressionToken_Vector(tokenVec);
		print_ExpressionTree(tree);
		OperandVec *resVec = evaluate_ExpressionTree(tree);
		OPERAND_VALUE_TYPE res; //default value
		if (resVec){
			res = resVec->values[0] ;
			char out[256]; out[255] = 0;
			sprintf(out,"%f",res);
			if ( strcmp(tests[i].output,out) ){
				fail++;
				printf("\033[31mFAIL\033[0m test %d: expected %s but got %s\n",i,tests[i].output,out);
			}
			else{
				printf("\033[7m = %s\033[0m",tests[i].output);
			}
		}
		else{
			fail++;
			printf("\033[31mFAIL\033[0m test %d: expected %s but failed to evaluate expression\n",i,tests[i].output);
		}
		
		free(es.str);
		//free(tokenVec->array);
		free_ExpressionToken_Vector(tokenVec);
		if (tree) free_ExpressionTreeNode(tree);
		if (resVec) free_OperandVec(resVec);
		printf("\n");
	}

	if (!fail){
		printf("\n\033[32mPASSED\033[0m all %d tests\n\n",testsCount);
	}
	else{
		printf("\n\033[31mFAILED\033[0m %d tests out of %d (%.2f%%)\n\n",fail,testsCount,100.0*((float)fail)/((float)testsCount));
	}
	
	return;
}


