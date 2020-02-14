#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
 
double PI;
 
double complex* in;
double complex* ou;
int N, P;
 
void merge_odd_even(double complex* buff, double complex* out, int n, int step, int deviation) {
        for (int i = 0; i < n; i += 2 * step) {
                double complex t = cexp(-I * PI * i / n) * out[i + step + deviation];
                buff[i / 2 + deviation] = out[i + deviation] + t;
                buff[(i + n)/2 + deviation] = out[i + deviation] - t;
        }
}
 
void _fft(double complex* buf, double complex* out, int n, int step)
{
    if(step >= n) {
        return;
    }
    _fft(out, buf, n, step * 2);
    _fft(out + step, buf + step, n, step * 2);
    merge_odd_even(buf, out, n, step, 0);  
}
 
void* threadFunction(void *a) {
    int id = *((int *)a);
    int step = P;
    double complex* input = in;
    double complex* output = ou;
    if(P == 2) {
        double complex* temp = input;
        input = output;
        output = temp;
    }
    _fft(input + id, output + id, N, step);
    return NULL;
}
 
void merge_step_main() {
    if(P == 4) {
            merge_odd_even(ou, in, N, 2, 0);
                merge_odd_even(ou, in, N, 2, 1);
        }
 
        if(P >= 2) {
                merge_odd_even(in, ou, N, 1, 0);
        }
}
 
void read_input(char* path) {
    int i = 0;
    double realPart = 0;
    FILE* input = fopen(path, "r");
    fscanf(input, "%d", &N);
    const int dim = N * sizeof(double complex);
    in = (double complex *)malloc(dim);
        ou = (double complex *)malloc(dim);
    for(i = 0; i < N; ++i) {
        fscanf(input, "%lf", &realPart);
        in[i] = realPart;
    }
    memcpy(ou, in, dim);
    fclose(input);
}
 
void print_output(char* path) {
        int i = 0;
    double realPart = 0, imaginaryPart = 0;
        FILE* output = fopen(path, "w");
        fprintf(output, "%d\n", N);
        for(i = 0; i < N; ++i) {
        realPart = creal(in[i]);
        imaginaryPart = cimag(in[i]);
                fprintf(output, "%lf %lf\n", realPart, imaginaryPart);
        }
        fclose(output);
}
 
int getNumThreads(char* num) {
    return atoi(num);
}
 
int main(int argc, char * argv[]) {
    int i = 0;
    PI = atan2(1, 1) * 4;
    read_input(argv[1]);
    P = getNumThreads(argv[3]);
 
    int threadIds[P];
    pthread_t t[P];
 
    for(i = 0; i < P; ++i) {
        threadIds[i] = i;
        pthread_create(&t[i], NULL, threadFunction, &threadIds[i]);
    }
 
    for(i = 0; i < P; ++i) {
        pthread_join(t[i], NULL);
    }
 
    merge_step_main();
 
    print_output(argv[2]);
 
    free(in);
    free(ou);
    return 0;
}