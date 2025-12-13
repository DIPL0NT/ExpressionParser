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

	//CleanUp
	free_SymbolTreeNode(operatorsSymbolTree);

	return 0;
}



