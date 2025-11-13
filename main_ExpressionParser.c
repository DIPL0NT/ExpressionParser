#include "ExpressionParser.c"

int main(){
	
	if (!checkCompatibilityOperatorAndOperandChars()){
		return 1;
	}

	print_avalaible_Operators();

	char input[] = "sqrt(0.1/(4**.2)+5.7)";
	printf("Input: %s\n",input);
	ExpressionString es = create_ExpressionString(input);
	printf("ExpressionString: %s\n",es.str);
	ExpressionElement el = {NULLTERM,NULL};
	printf("Parsed tokens: ");
	while ( (el = get_next_ExpressionElement_from_ExpressionString(&es)).type!=NULLTERM ){
		print_ExpressionElement(el);
		printf(" ");
	}
	printf("\n");

	return 0;
}





