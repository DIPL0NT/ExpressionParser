typedef struct fraction{
    int num;
    int den;   
}fraction;

extern fraction create_fraction(int num,int den);

extern void print_fraction(fraction f);

extern void simplify_fraction(fraction *f);

extern int sign_fraction(fraction f);

extern fraction simplified_fraction(fraction f);

extern fraction sum_fractions(fraction f1,fraction f2);

extern fraction sub_fractions(fraction f1,fraction f2);

extern int compare_fractions(fraction f1, fraction f2);

extern fraction mult_fractions(fraction f1,fraction f2);

extern fraction div_fractions(fraction f1,fraction f2);



