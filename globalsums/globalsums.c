/*
 *  Copyright (c) 2015, Los Alamos National Security, LLC.
 *  All rights Reserved.
 *
 *  Distributed under the OSI Certified Apache License 2.0
 *
 *  Extracted from GlobalSums, Version 1.0.0 (C16001) -- LA-CC-15-087
 *
 *  Author -- Bob Robey, brobey@lanl.gov
 *
 *  ABSTRACT
 *  A demonstration code to support a paper Computational Reproducibility for
 *  Production Physics Applications submitted to the Numerical Reproducibility
 *  at Exascale (NRE 2015) workshop at the 2015 Supercomputing conference, Nov 20, 2015.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <immintrin.h>
#include "timer.h"

#define ORDERS_OF_MAGNITUDE 1.0e9;

#define MIN(a,b) (((a)<(b))?(a):(b))

typedef unsigned int uint;

void do_kahan_sum(double *var, long ncells, double accurate_sum);
void do_kahan_sum_v(double *var, long ncells, double accurate_sum);

int main(int argc, char *argv[])
{

  for (int pow_of_two = 4; pow_of_two < 31; pow_of_two++){
      long ncells = (long)pow((double)2,(double)pow_of_two);
      long ncellsdiv2 = ncells/2;

      double high_value = 1.0e-1;
      double low_value  = 1.0e-1/ORDERS_OF_MAGNITUDE;
      double accurate_sum = (double)ncellsdiv2 * high_value +
                            (double)ncellsdiv2 * low_value;

      double *energy = (double *)malloc(ncells*sizeof(double));

      // Initialize with high values first
      printf("Initializing mesh with Leblanc problem, high values first\n");
      for (long i = 0; i < ncells; i++){
         energy[i] = (i < ncellsdiv2) ? high_value : low_value;
      }

      do_kahan_sum(energy, ncells, accurate_sum);

      do_kahan_sum_v(energy, ncells, accurate_sum);

      printf("\n");

      free(energy);
   }
}
   
void do_kahan_sum(double *var, long ncells, double accurate_sum)
{
   struct timeval cpu_timer;
   cpu_timer_start(&cpu_timer);

   struct esum_type{
      double sum;
      double correction;
   } local;
   local.sum = 0.0;
   local.correction = 0.0;

   for (long i = 0; i < ncells; i++) {
      double corrected_next_term= var[i] + local.correction;
      double new_sum            = local.sum + local.correction;
      local.correction   = corrected_next_term - (new_sum - local.sum);
      local.sum          = new_sum;
   }
   double sum = local.sum + local.correction;

   double cpu_time = cpu_timer_stop(cpu_timer);
   
   printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
          accurate_sum,sum,(sum-accurate_sum),((sum-accurate_sum)/accurate_sum), cpu_time);
   printf("   Serial sum with double double kahan sum accumulator\n");
}

void do_kahan_sum_v(double *var, long ncells, double accurate_sum)
{
   struct timeval cpu_timer;
   cpu_timer_start(&cpu_timer);

   double const zero = 0.0;
   double *sum_v;
   posix_memalign((void **)&sum_v, 64, sizeof(double)*4);
   __m256d local_sum = _mm256_broadcast_sd((double const*) &zero);
   __m256d local_correction = _mm256_broadcast_sd((double const*) &zero);
   __m256d sum = _mm256_broadcast_sd((double const*) &zero);

   #pragma omd simd aligned
   for (long i = 0; i < ncells; i+=4) {
       __m256d var_v = _mm256_load_pd(&var[i]);
       __m256d corrected_next_term = var_v + local_correction;
       __m256d new_sum = local_sum + local_correction;
       local_correction = corrected_next_term - (new_sum - local_sum);
       local_sum = new_sum;
   }
   sum += local_correction;
   sum += local_sum;
   _mm256_store_pd(sum_v, sum);

   struct esum_type{
      double sum;
      double correction;
   } local;
   local.sum = 0.0;
   local.correction = 0.0;

   for (long i = 0; i < 4; i++) {
      double corrected_next_term_s= sum_v[i] + local.correction;
      double new_sum_s            = local.sum + local.correction;
      local.correction   = corrected_next_term_s - (new_sum_s - local.sum);
      local.sum          = new_sum_s;
   }
   double final_sum = local.sum + local.correction;
	
   free(sum_v);

   double cpu_time = cpu_timer_stop(cpu_timer);
   
   printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
          accurate_sum,final_sum,(final_sum-accurate_sum),((final_sum-accurate_sum)/accurate_sum), cpu_time);
   printf("   Vectorized sum with double double kahan sum accumulator\n");
}

