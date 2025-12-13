#include "fractions.h"
#include <stdio.h>
#include <stdlib.h>

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



fraction *alloc_fraction(int num,int den){
	//if (den==0) ...
	fraction *f = malloc(sizeof(fraction));
	//if (!f) ...
	f->num = num;
	f->den = den;
	return f;
}

void free_fraction(fraction *f){
	//if (!f) ...
	free(f);
	return;
}

void print_fraction(fraction *f){
	printf("%d/%d",f->num,f->den);
	return;
}

void simplify_fraction(fraction *f){
	//if (!f) ...
	//if (f->den==0) ...
	int d = mcd(f->num,f->den);
	f->num = f->num / d *sign(f->den);
	f->den = abs(f->den / d);
	return;
}

int sign_fraction(fraction *f){
	//if (!f) ...
	return sign(f->num)*sign(f->den);
}

fraction *simplified_fraction(fraction *f){
	//if (!f) ...
	//if (f->den==0) ...
	fraction *newf = malloc(sizeof(fraction));
	//if (!newf) ...
	int d = mcd(f->num,f->den);
	newf->num = f->num / d *sign(f->den);
	newf->den = abs(f->den / d);
	return newf;
}

fraction *sum_fractions(fraction *f1,fraction *f2){
	//if (!f1 || !f2) ...
	simplify_fraction(f1);
	simplify_fraction(f2);
	fraction *newf = malloc(sizeof(fraction));
	//if (!newf) ...
	newf->den = mcm(f1->den,f2->den);
	//if (newf.den==0) ...
	newf->num = (newf->den/f1->den) * f1->num
				+ (newf->den/f2->den) * f2->num;
	return newf;
}

fraction *sub_fractions(fraction *f1,fraction *f2){
	//if (!f1 || !f2) ...
	simplify_fraction(f1);
	simplify_fraction(f2);
	fraction *newf = malloc(sizeof(fraction));
	//if (!newf) ...
	newf->den = mcm(f1->den,f2->den);
	//if (newf.den==0) ...
	newf->num = (newf->den/f1->den) * f1->num
				- (newf->den/f2->den) * f2->num;
	return newf;
}

int compare_fractions(fraction *f1, fraction *f2){
	//if (!f1 || !f2) ...
	fraction *newf = sub_fractions(f1,f2);
	//if (!newf) ...
	if (newf->num==0) return 0;
	int sign = sign_fraction(newf);
	free_fraction(newf);
	return sign;
}

fraction *mult_fractions(fraction *f1,fraction *f2){
	//if (!f1 || !f2) ...
	simplify_fraction(f1);
	simplify_fraction(f2);
	fraction *newf = alloc_fraction(f2->num,f1->den);
	//if (!newf) ...
	fraction *resf1 = simplified_fraction( newf );
	free_fraction(newf);
	//if (!resf1) ...
	newf = alloc_fraction(f1->num,f2->den);
	//if (!newf) ...
	fraction *resf2 = simplified_fraction( newf );
	free_fraction(newf);
	//if (!resf2) ...
	newf = alloc_fraction( resf1->num*resf2->num , resf1->den*resf2->den );
	free_fraction(resf1);
	free_fraction(resf2);
	//if (!newf) ...
	return newf;
}

fraction *div_fractions(fraction *f1,fraction *f2){
	//if (!f1 || !f2) ...
	fraction *newf = alloc_fraction(f2->den,f2->num);
	//if (!newf) ...
	fraction *resf = mult_fractions(f1, newf );
	free_fraction(newf);
	//if (!resf) ...
	return resf;
}




