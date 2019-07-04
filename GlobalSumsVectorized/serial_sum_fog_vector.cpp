#include "vectorclass.h"

static double sum[4] __attribute__ ((aligned (64)));

extern "C" {
double do_serial_sum_fog_v(double* var, long ncells);
}

double do_serial_sum_fog_v(double* var, long ncells)
{

   Vec4d local_sum = {0.0};

   for (long i = 0; i < ncells; i+=4) {
       Vec4d var_v = *(Vec4d *)&var[i];
       local_sum += var_v;
   }
   local_sum.store(sum);

   double final_sum = 0.0;
   for (long i = 0; i < 4; i++) {
      final_sum += sum[i];
   }
   return(final_sum);
}
