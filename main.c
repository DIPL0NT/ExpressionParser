#include "parser.c"

int main(int argc,char** argv){

	//very basic
	fraction f1;
	fraction f2;
	char operation;

	/*
	while(1){
		printf("Type fraction1 numerator:   ");
		scanf("%d",&f1.num);
		printf("                            ---\n");
		printf("Type fraction1 denominator: ");
		scanf("%d",&f1.den);
		scanf("%c",&operation); //to absorb \n char
		printf("Type operation:             ");
		scanf("%c",&operation);
		printf("Type fraction2 numerator:   ");
		scanf("%d",&f2.num);
		printf("                            ---\n");
		printf("Type fraction2 denominator: ");
		scanf("%d",&f2.den);
		printf("Result:                     ");
		switch (operation){
			case '+':
				print_fraction(sum_fractions(f1,f2));
				break;
			case '-':
				print_fraction(sub_fractions(f1,f2));
				break;
			case '*':
				print_fraction(mult_fractions(f1,f2));
				break;
			case '/':
				print_fraction(div_fractions(f1,f2));
				break;
		}
		printf("\n\n");
	}
	*/
	printf("Type expression in format: <num>/<den> <operator> <num>/<den>\n");
	printf("Available operators: '+' '-' '*' '/'\n");
	printf("Operator 's' to semplify the two fractions\n");
	while (1){
		scanf("%d/%d %c %d/%d",&f1.num,&f1.den,&operation,&f2.num,&f2.den);
		printf(" = ");
		switch (operation){
			case 's':
				print_fraction(simplified_fraction(f1));
				printf(" and ");
				print_fraction(simplified_fraction(f2));
				break;
			case '+':
				print_fraction(sum_fractions(f1,f2));
				break;
			case '-':
				print_fraction(sub_fractions(f1,f2));
				break;
			case '*':
				print_fraction(mult_fractions(f1,f2));
				break;
			case '/':
				print_fraction(div_fractions(f1,f2));
				break;
		}
		printf("\n");
	}


	return 0;
}

