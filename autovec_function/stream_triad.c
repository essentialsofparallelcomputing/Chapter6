#include <stdio.h>
#include "timer.h"

#define NTIMES 16
#define STREAM_ARRAY_SIZE 80000000
static double a[STREAM_ARRAY_SIZE], b[STREAM_ARRAY_SIZE], c[STREAM_ARRAY_SIZE];

void stream_triad(double* a, double* b, double* c, double scalar){
   for (int i=0; i<STREAM_ARRAY_SIZE; i++){
      a[i] = b[i] + scalar*c[i];
   }
}

int main(int argc, char *argv[]){
   struct timespec tstart;
   double scalar = 3.0, time_sum = 0.0;
   for (int i=0; i<STREAM_ARRAY_SIZE; i++) {
      a[i] = 1.0;
      b[i] = 2.0;
   }

   for (int k=0; k<NTIMES; k++){
      cpu_timer_start(&tstart);
      stream_triad(a, b, c, scalar);
      time_sum += cpu_timer_stop(tstart);
      // to keep the compiler from optimizing out the loop
      c[1] = c[2];
   }
   printf("Average runtime is %lf msecs\n", time_sum/NTIMES);
}
