#ifndef _TIMER_H
#define _TIMER_H
#define REAL_CELL 1

double timestep(int ncells, double g, double sigma, int *celltype,
                double *H, double *U, double *V, double *dx, double *dy);
#endif
