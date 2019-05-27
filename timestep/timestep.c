#include <math.h>
#include "timestep.h"
#define NCELLS 10000000
#define REAL_CELL 1

double timestep(int ncells, double g, double sigma, int* restrict celltype,
                double* H, double* U, double* V, double* dx, double* dy){
   double wavespeed, xspeed, yspeed, dt;
   double mymindt = 1.0e20;
   for (int ic=0; ic<NCELLS ; ic++) {
      if (celltype[ic] == REAL_CELL) {
         wavespeed = sqrt(g*H[ic]);
         xspeed = (fabs(U[ic])+wavespeed)/dx[ic];
         yspeed = (fabs(V[ic])+wavespeed)/dy[ic];
         dt=sigma/(xspeed+yspeed);
         if (dt < mymindt) mymindt = dt;
      }
   }
   return(mymindt);
}

