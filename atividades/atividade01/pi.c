/*
 * pi.c
 *
 *  Created on: 10 de mar de 2019
 *      Author: robertcabral
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float monteCarloPi(int n){

	int i;
	float acertos = 0, x, y;
	for(i = 0; i < n; i++){
		x = ((float)rand()/(float)(RAND_MAX));
		y = ((float)rand()/(float)(RAND_MAX));
		
		if((x * x) + (y * y) < 1)
			acertos += 1;
	}
	return (4 * acertos / n);
}

int main(int argc, char *argv[]){
	
	srand(time(NULL));

	if(argc == 2)
		printf("pi = %.5f\n", monteCarloPi(atoi(argv[1])));
	
	return 0;

}
