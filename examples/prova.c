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


int main(){
	float res = sumArgsTest(2);
	printf("Res=%f\n",res);
	printf("Type \"/0.1\" :\n");
	scanf("%f",&res);
	printf("I scanned the float %f\n",res);
	return 0;
}