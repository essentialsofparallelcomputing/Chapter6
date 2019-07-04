static double sum[4] __attribute__ ((aligned (64)));

double do_serial_sum_gcc_v(double* restrict var, long ncells)
{
   typedef double vec4d __attribute__ ((vector_size(4 * sizeof(double))));

   vec4d local_sum = {0.0};

   for (long i = 0; i < ncells; i+=4) {
       vec4d var_v = *(vec4d *)&var[i];
       local_sum += var_v;
   }
   *(vec4d *)sum = local_sum;

   double final_sum = 0.0;
   for (long i = 0; i < 4; i++) {
      final_sum += sum[i];
   }
   return(final_sum);
}
