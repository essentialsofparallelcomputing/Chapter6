#include <x86intrin.h>

static double sum[8] __attribute__ ((aligned (64)));

double do_serial_sum_intel_v8(double* restrict var, long ncells)
{
   __m512d local_sum = {0.0};

   for (long i = 0; i < ncells; i+=8) {
       __m512d var_v = _mm512_load_pd(&var[i]);
       local_sum += var_v;
   }
   _mm512_store_pd(sum, local_sum);

   double final_sum = 0.0;
   for (long i = 0; i < 8; i++) {
      final_sum += sum[i];
   }
   return(final_sum);
}
