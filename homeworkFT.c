#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#define _USE_MATH_DEFINES // for C
#include <math.h>
#include <pthread.h>

#define _USE_MATH_DEFINES // for C

int N, P;
double *val;
double complex *rez;
void* fthread(void* args) {

	int j, k;
	double complex op, p;
	int id = *((int*)args);
	int length, poz1, poz2;

	length = N % P == 0 ? 
		N / P : 
		N / P + 1;

	poz1 = id * length;
	poz2 = poz1 + length;

	poz2 = poz2 > N ? N : poz2;

	for(k = poz1; k < poz2; k++) {
		for(j = 0; j < N; j++) {
			op = (( (-2) * I * M_PI ) / N ) * k * j;
			p = cpow(M_E, op);
			rez[k] = rez[k] + val[j] * p;
			
		}
	
	}
	return NULL;
}
int main(int argc, char * argv[]) {

	FILE *f = fopen(argv[1], "rt");
	int i, ret;

	P = atoi(argv[3]);
	ret = fscanf(f, "%d", &N);

	if(ret != 1) {
			fprintf(stdout, "Failed to read value");
			exit(1);
		}

	val = malloc(N * sizeof(double));
	rez = calloc(N, sizeof(double complex));

	for(i = 0; i < N; i++) {
		ret = fscanf(f, "%lf", &val[i]);
		if(ret != 1) {
			fprintf(stdout, "Failed to read value");
			exit(1);
		}
	}
	

	FILE *g = fopen(argv[2], "wt");
	fprintf(g, "%d\n", N);

	pthread_t threads[P];
    	int id[P];

	for(i = 0; i < P; i++)
		id[i] = i;

	for(i = 0; i < P; i++)
		pthread_create(&threads[i], NULL, fthread, &id[i]);

	for(i = 0; i < P; ++i)
		pthread_join(threads[i], NULL);
	
	for(i = 0; i < N; i++)
		fprintf(g, "%f %f\n", creal(rez[i]), cimag(rez[i]));
	

	fclose(f);
	fclose(g);

	return 0;
}
