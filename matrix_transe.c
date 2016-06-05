#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

double calculate(int n) {
  double* a = (double*)malloc(n * n * sizeof(double)); // Matrix A
  double* b = (double*)malloc(n * n * sizeof(double)); // Matrix B
  double* c = (double*)malloc(n * n * sizeof(double)); // Matrix C
  double* bt = (double*)malloc(n * n * sizeof(double)); // Matrix B^t

  // Initialize the matrices to some values.
  int i, j, k;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      a[i * n + j] = i * n + j; // A[i][j]
      b[i * n + j] = j * n + i; // B[i][j]
      c[i * n + j] = 0; // C[i][j]
    }
  }

  double begin = get_time();

  /* Write code to calculate C = A * B. */
  for (i = 0; i < n; i++) {
    for (j = 0; j< n; j++) {
      bt[i * n + j] = b[j * n + i];
    }
  }
  free(b);
  for (i = 0; i < n; i++) {
    for (k = 0; k < n; k++){
      for (j = 0; j < n; j++) {
        c[i * n + k] += a[i * n + j] * bt[k * n + j];
      }
    }
  }

  double end = get_time();
  /* printf("time: %.6lf sec\n", end - begin); */

  // Print C for debugging. Comment out the print before measuring the execution time.
  double sum = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      sum += c[i * n + j];
    }
  }
  // Print out the sum of all values in C.
  // This should be 450 for N=3, 3680 for N=4, and 18250 for N=5.
  /* printf("sum: %.6lf\n", sum); */

  free(a);
  free(bt);
  free(c);
  return end - begin;
}

int main()
{
  double sum;
  int n, i;
  for (n = 2; n < 500;){
    sum = 0;
    for (i = 0; i < 50; i++) {
      sum += calculate(n);
    }
    printf("%d %.6lf\n", n, sum/50.0);
    n += 10;
  }
  return 0;
}
