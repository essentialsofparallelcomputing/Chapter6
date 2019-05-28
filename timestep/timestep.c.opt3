#include <math.h>
#include "timestep.h"
#define REAL_CELL 1

double timestep(int ncells, double g, double sigma, int* celltype,
                double* H, double* U, double* V, double* dx, double* dy){
   double mymindt = 1.0e20;
#pragma omp simd reduction(min:mymindt)
   for (int ic=0; ic<ncells ; ic++) {
      if (celltype[ic] == REAL_CELL) {
         double wavespeed = sqrt(g*H[ic]);
         double xspeed = (fabs(U[ic])+wavespeed)/dx[ic];
         double yspeed = (fabs(V[ic])+wavespeed)/dy[ic];
         double dt=sigma/(xspeed+yspeed);
         if (dt < mymindt) mymindt = dt;
      }
   }
   return(mymindt);
}

