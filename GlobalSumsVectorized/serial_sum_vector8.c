static double sum[8] __attribute__ ((aligned (64)));

double do_serial_sum_v8(double* restrict var, long ncells)
{
   typedef double vec8d __attribute__ ((vector_size(8 * sizeof(double))));

   vec8d local_sum = {0.0};
   vec8d var_v;

   for (long i = 0; i < ncells; i+=8) {
       var_v = *(vec8d *)&var[i];
       local_sum += var_v;
   }
   *(vec8d *)sum = local_sum;

   double final_sum = 0.0;
   for (long i = 0; i < 8; i++) {
      final_sum += sum[i];
   }
   return(final_sum);
}
