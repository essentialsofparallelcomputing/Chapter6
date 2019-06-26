double do_sum(double* restrict var, long ncells)
{
   // Serial sum
   double sum = 0.0;
   for (long i = 0; i < ncells; i++){
      sum += var[i];
   }

   return(sum);
}
