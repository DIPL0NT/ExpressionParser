#include <stdio.h>
#include <stdlib.h>

float sumFuncArgVec(float args[2]){
	return args[0] + args[1] ;
}

float sumArgsTest(int vec_size){
	float args[vec_size];
	args[0] = 2;
	args[1] = 2;
	return sumFuncArgVec(args);
}

typedef struct args{
	int a;
	int b;
	int c;
	int d;
} args;


int main(){
	/*
	float res = sumArgsTest(2);
	printf("Res=%f\n",res);
	printf("Type \"/0.1\" :\n");
	scanf("%f",&res);
	printf("I scanned the float %f\n",res);
	*/

	/*
	float a = 2.0f;
	float b = 2.0f;
	float args[2]; args[0]=a; args[1]=b;
	printf("%f %s %f = %f \n",a,sumOp.symbol,b, sumOp.function(args) );   //( (float(*)(float,float))sumOp.function )(a,b) );
	*/

	args X = {1,2,3,4};

	args Y = X ;

	printf("%d %d %d %d\n",Y.a,Y.b,Y.c,Y.d);

	return 0;
}