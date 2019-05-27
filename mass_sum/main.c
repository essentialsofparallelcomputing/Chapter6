#include <stdio.h>
#include "mass_sum.h"
#define NCELLS 10000000
static double H[NCELLS], dx[NCELLS], dy[NCELLS];
static int celltype[NCELLS];

int main(int argc, char *argv[]){
   double summer;
   for (int ic=0; ic<NCELLS ; ic++) {
      H[ic] = 10.0;
      dx[ic] = 0.5;
      dy[ic] = 0.5;
      celltype[ic] = REAL_CELL;
   }
   H[NCELLS/2] = 20.0;

   summer = mass_sum(NCELLS, celltype, H, dx, dy);

   printf("Mass Sum is %lf\n",summer);
}

