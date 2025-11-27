#include "ExpressionTree.c"

int main(){
	
	if (!checkCompatibilityOperatorAndOperandChars()){
		return 1;
	}

	print_avalaible_Operators();

	char input[] = "sqrt( 0.1/(4**.2) +5.7)";
	printf("Input: %s\n",input);
	ExpressionString es = create_ExpressionString(input);
	printf("ExpressionString: %s\n",es.str);
	ExpressionToken tok = {NULLTERM,NULL};
	printf("Parsed tokens: ");
	while ( (tok = get_next_ExpressionToken_from_ExpressionString(&es)).type!=NULLTERM ){
		print_ExpressionToken(tok);
		printf("_");
	}
	printf("\n");

	float a = 2.0f;
	float b = 2.0f;
	float args[2]; args[0]=a; args[1]=b;
	printf("%f %s %f = %f \n",a,sumOp.symbol,b, sumOp.function(args) );   //( (float(*)(float,float))sumOp.function )(a,b) );

	return 0;
}





