#include <stdio.h>
#include <stdlib.h>
//#include <math.h>

int abs(int x){
	return x<0 ? -x : x ;
}

int min(int x,int y){
	return x<y ? x : y ;
}

int max(int x,int y){
	return x>y ? x : y ;
}

int sign(int x){
	return x<0 ? -1 : +1 ;
}

int mcd(int x,int y){
	if (x==0 && y==0) return 1;
	int abs_x = abs(x);
	int abs_y = abs(y);
	if (abs_x==0 || abs_y==0) return max(abs_x,abs_y);
	if (abs_x==1 || abs_y==1) return 1;
	for (int d=min(abs_x,abs_y) ; d>1 ; d--){
		if (abs_x%d==0 && abs_y%d==0){
			return d;
		}
	}
	return 1; //should never get here anyway right?
}

int mcm(int x,int y){
	int mcm_sign = sign(x)*sign(y);
	int abs_x = abs(x);
	int abs_y = abs(y);
	int mcm = max(abs_x,abs_y);
	for ( ; /*mcm<abs_x*abs_y*/ ; mcm++){
		if (mcm%abs_x==0 && mcm%abs_y==0){
			return mcm_sign*mcm;
		}
	}
	return mcm_sign*mcm;
}

typedef struct fraction{
	int num;
	int den;
} fraction;

fraction create_fraction(int num,int den){
	//if (den==0) ...
	fraction f;
	f.num = num;
	f.den = den;
	return f;
}

void print_fraction(fraction f){
	printf("%d/%d",f.num,f.den);
	return;
}

void simplify_fraction(fraction *f){
	//if (!f) return;
	//if (f->den==0) ...
	int d = mcd(f->num,f->den);
	f->num = f->num / d *sign(f->den);
	f->den = abs(f->den / d);
	return;
}

int sign_fraction(fraction f){
	return sign(f.num)*sign(f.den);
}

fraction simplified_fraction(fraction f){
	//if (f.den==0) ...
	int d = mcd(f.num,f.den);
	f.num = f.num / d *sign(f.den);
	f.den = abs(f.den / d);
	return f;
}

fraction sum_fractions(fraction f1,fraction f2){
	simplify_fraction(&f1);
	simplify_fraction(&f2);
	fraction newf;
	newf.den = mcm(f1.den,f2.den);
	//if (newf.den==0) ...
	newf.num = (newf.den/f1.den) * f1.num
				+ (newf.den/f2.den) * f2.num;
	return newf;
}

fraction sub_fractions(fraction f1,fraction f2){
	simplify_fraction(&f1);
	simplify_fraction(&f2);
	fraction newf;
	newf.den = mcm(f1.den,f2.den);
	//if (newf.den==0) ...
	newf.num = (newf.den/f1.den) * f1.num
				- (newf.den/f2.den) * f2.num;
	return newf;
}

int compare_fractions(fraction f1, fraction f2){
	f1 = sub_fractions(f1,f2);
	if (f1.num==0) return 0;
	return sign_fraction(f1);
}

fraction mult_fractions(fraction f1,fraction f2){
	simplify_fraction(&f1);
	simplify_fraction(&f2);
	int f1_num = f1.num;
	f1 = simplified_fraction(
			create_fraction(f2.num,f1.den) );
	f2 = simplified_fraction(
				create_fraction(f1_num,f2.den) );
	f1.num = f1.num * f2.num;
	f1.den = f1.den * f2.den;
	return f1;
}

fraction div_fractions(fraction f1,fraction f2){
	return mult_fractions(
			f1, create_fraction(f2.den,f2.num) );
}



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

