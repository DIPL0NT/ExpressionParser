#include "ExpressionTree_Evaluation.c"

typedef struct ExpressionTestCase{
	char *input;
	char *output;
} ExpressionTestCase;

void runExpressionTests(ExpressionTestCase *tests,int testsCount){

	int fail = 0;
	ExpressionString es;
	ExpressionToken_Vector *tokenVec;
	ExpressionTreeNode *tree;
	OPERAND_VALUE_TYPE res;
	OperandVec_Wrapper res_wrp;
	for (int i=0;i<testsCount;i++){
		printf("\n");
		char *input = tests[i].input;
		printf("\033[7m%s\033[0m\n",input);
		es = create_ExpressionString(input);
		tokenVec = create_ExpressionToken_Vector_from_ExpressionString(operatorsSymbolTree,&es);
		print_ExpressionTokenVector(tokenVec);
		tree = create_ExpressionTree_from_ExpressionToken_Vector(tokenVec);
		print_ExpressionTree(tree);
		res_wrp = evaluate_ExpressionTree(tree);
		if (!is_OperandVec_Wrapper_NULL(res_wrp)){
			res = res_wrp.vec->values[0] ;
			char out[256]; out[255] = 0;
			sprint_OperandValue(out,res);
			if ( strcmp(tests[i].output,out) ){
				fail++;
				printf("\033[7m = %s\033[0m\n",out);
				printf("\033[31mFAIL\033[0m test %d: \033[7mexpected %s but got %s\033[0m\n",i,tests[i].output,out);
			}
			else{
				printf("\033[7m = %s\033[0m\n",tests[i].output);
				printf("\033[32mPASS\033[0m test %d\n",i);
			}
		}
		else{
			if ( !strcmp(tests[i].output,"evaluate_ExpressionTree(NULL)") ){
				printf("\033[7m = %s\033[0m",tests[i].output);
				printf("\033[32mPASS\033[0m test %d\n",i);
			}
			else {
				fail++;
				printf("\033[31mFAIL\033[0m test %d: \033[7mexpected %s but failed to evaluate expression\033[0m\n",i,tests[i].output);
			}
		}
		
		free(es.str);
		//free(tokenVec->array);
		free_ExpressionToken_Vector(tokenVec);
		if (tree) free_ExpressionTreeNode(tree);
		if (!is_OperandVec_Wrapper_NULL(res_wrp)){
			release_OperandVec_Wrapper_values(res_wrp);
			free_OperandVec_Wrapper(res_wrp);
		}
		printf("\n");
	}

	if (!fail){
		printf("\n\033[32mPASSED\033[0m all %d tests\n\n",testsCount);
	}
	else if (testsCount){
		printf("\n\033[31mFAILED\033[0m %d tests out of %d (%.2f%%)\n\n",fail,testsCount,100.0*((float)fail)/((float)testsCount));
	}
	else{
		printf("\n\033[31mNO TESTS\033[0m testCount = 0\n\n");
	}
	
	return;
}

