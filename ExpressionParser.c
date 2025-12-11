#include "ExpressionTree_Evaluation.c"

typedef struct testCase{
	char *input;
	char *output;
} testCase;


void runTests(ExpressionContext *context, testCase *tests, int testsCount){

    if (!context || !tests || !testsCount) return;

	int fail = 0;
	for (int i=0;i<testsCount;i++){
		char *input = tests[i].input;
		ExpressionString es = create_ExpressionString(input);
		ExpressionToken_Vector *tokenVec = create_ExpressionToken_Vector_from_ExpressionString(context,&es);
		ExpressionTreeNode *tree = create_ExpressionTree_from_ExpressionToken_Vector(tokenVec);
		OperandVec *resVec = evaluate_ExpressionTree(tree);
		OPERAND_VALUE_TYPE res = 0.0; //default value
		if (resVec){
			res = resVec->values[0] ;
			char out[256]; out[255] = 0;
			sprintf(out,"%f",res);
			if ( strcmp(tests[i].output,out) ){
				fail++;
				printf("\033[31mFAIL\033[0m test %d: expected %s but got %s\n",i,tests[i].output,out);
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
	}

	if (!fail){
		printf("\033[32mPASSED\033[0m all %d tests\n",testsCount);
	}
	else{
		printf("\033[31mFAILED\033[0m %d tests out of %d (%.2f%%)\n",fail,testsCount,100.0*((float)fail)/((float)testsCount));
	}
	
	return;
}




