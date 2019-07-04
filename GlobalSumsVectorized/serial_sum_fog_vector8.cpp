#include "vectorclass.h"

static double sum[8] __attribute__ ((aligned (64)));

extern "C" {
double do_serial_sum_fog_v8(double* var, long ncells);
}

double do_serial_sum_fog_v8(double* var, long ncells)
{

   Vec8d local_sum = {0.0};

   for (long i = 0; i < ncells; i+=8) {
       Vec8d var_v = *(Vec8d *)&var[i];
       local_sum += var_v;
   }
   local_sum.store(sum);

   double final_sum = 0.0;
   for (long i = 0; i < 8; i++) {
      final_sum += sum[i];
   }
   return(final_sum);
}
