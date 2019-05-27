#include <stdio.h>
#include "timestep.h"
#define NCELLS 10000000
static double H[NCELLS], U[NCELLS], V[NCELLS], dx[NCELLS], dy[NCELLS];
static int celltype[NCELLS];

int main(int argc, char *argv[]){
   double g = 9.80, sigma = 0.95;
   double mymindt;
   for (int ic=0; ic<NCELLS ; ic++) {
      H[ic] = 10.0;
      U[ic] = 0.0;
      V[ic] = 0.0;
      dx[ic] = 0.5;
      dy[ic] = 0.5;
      celltype[ic] = REAL_CELL;
   }
   H[NCELLS/2] = 20.0;

   mymindt = timestep(NCELLS, g, sigma, celltype, H, U, V, dx, dy);

   printf("Minimum dt is %lf\n",mymindt);
}

