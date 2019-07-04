#include <x86intrin.h>

static double sum[4] __attribute__ ((aligned (64)));

double do_serial_sum_intel_v(double* restrict var, long ncells)
{
   __m256d local_sum = {0.0};

   for (long i = 0; i < ncells; i+=4) {
       __m256d var_v = _mm256_load_pd(&var[i]);
       local_sum += var_v;
   }
   _mm256_store_pd(sum, local_sum);

   double final_sum = 0.0;
   for (long i = 0; i < 4; i++) {
      final_sum += sum[i];
   }
   return(final_sum);
}
