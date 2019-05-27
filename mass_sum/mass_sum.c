#include "mass_sum.h"
#define NCELLS 10000000
#define REAL_CELL 1

double mass_sum(int ncells, int* restrict celltype,
                double* restrict H, double* restrict dx, double* restrict dy){
   double summer = 0.0;
#pragma omp simd reduction(+:summer)
   for (int ic=0; ic<NCELLS ; ic++) {
      if (celltype[ic] == REAL_CELL) {
         summer += H[ic]*dx[ic]*dy[ic];
      }
   }
   return(summer);
}

