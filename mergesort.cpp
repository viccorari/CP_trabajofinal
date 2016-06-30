#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <thread>
#include "omp.h"

#define MAX_SIZE 100000000
#define MAX_T 2

using namespace std;

void generate_list(long * x, long n) {
   long i,j,t;
   for (i = 0; i < n; i++)
     x[i] = i;
   for (i = 0; i < n; i++) {
     j = rand() % n;
     t = x[i];
     x[i] = x[j];
     x[j] = t;
   }
}

void print_list(long * x, long n) {
   long i;
   for (i = 0; i < n; i++) {
      printf("%ld ",x[i]);
   } 
   printf("\n");
}

void merge(long * X, long n, long * tmp) {
   long i = 0;
   long j = n/2;
   long ti = 0;

   while (i<n/2 && j<n) {
      if (X[i] < X[j]) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; j++;
      }
   }
   while (i<n/2) { /* finish up lower half */
      tmp[ti] = X[i];
      ti++; i++;
   }
      while (j<n) { /* finish up upper half */
         tmp[ti] = X[j];
         ti++; j++;
   }
   memcpy(X, tmp, n*sizeof(long));

} // end of merge()

void mergesort(long * X, long n, long * tmp)
{
   if (n < 2) return;

   #pragma omp task firstprivate (X, n, tmp)
   mergesort(X, n/2, tmp);

   #pragma omp task firstprivate (X, n, tmp)
   mergesort(X+(n/2), n-(n/2), tmp);
 
   #pragma omp taskwait

    /* merge sorted halves into sorted list */
   merge(X, n, tmp);
}


int main()
{
   long n = MAX_SIZE;
   double start, stop;

   long data[MAX_SIZE], tmp[MAX_SIZE];
  
   generate_list(data, n);
   printf("List Before Sorting...\n");
   //print_list(data, n);
   start = omp_get_wtime();
   #pragma omp parallel
   {
      #pragma omp single
      mergesort(data, n, tmp);
   }
   stop = omp_get_wtime();
   printf("\nList After Sorting...\n");
   //print_list(data, n);
   printf("\nTime: %g\n",stop-start);
}

