#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

double do_sum_novec(double *var, long ncells);
double do_sum(double *var, long ncells);
double do_kahan_sum(double *var, long ncells);
double do_serial_sum_intel_v(double *var, long ncells);
double do_serial_sum_gcc_v(double *var, long ncells);
double do_serial_sum_fog_v(double *var, long ncells);
double do_serial_sum_intel_v8(double *var, long ncells);
double do_serial_sum_gcc_v8(double *var, long ncells);
double do_serial_sum_fog_v8(double *var, long ncells);
double do_kahan_sum_intel_v(double *var, long ncells);
double do_kahan_sum_gcc_v(double *var, long ncells);
double do_kahan_sum_agner_v(double *var, long ncells);
double do_kahan_sum_intel_v8(double *var, long ncells);
double do_kahan_sum_gcc_v8(double *var, long ncells);
double do_kahan_sum_agner_v8(double *var, long ncells);

void cpu_timer_start(struct timespec *tstart_cpu);
double cpu_timer_stop(struct timespec tstart_cpu);

#define ORDERS_OF_MAGNITUDE 1.0e9;

typedef unsigned int uint;

int main(int argc, char *argv[])
{
  for (int pow_of_two = 8; pow_of_two < 31; pow_of_two++){
      long ncells = (long)pow((double)2,(double)pow_of_two);
      long ncellsdiv2 = ncells/2;

      printf("SETTINGS INFO -- ncells %ld log %d\n",ncells,(int)log2((double)ncells));

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

      double test_sum;
      struct timespec cpu_timer;
      double cpu_time;

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_sum_novec(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Serial sum\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Kahan sum with double double accumulator\n");

//******************************************************
      printf(" 4 wide vectors serial sum\n");
//******************************************************

#ifndef HAVE_AVX512
      cpu_timer_start(&cpu_timer);

      test_sum = do_sum(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Serial sum (OpenMP SIMD pragma)\n");
#endif

//******************************************************

#ifdef HAVE_X86_64_INTRINSICS_XXX
      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_intel_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Intel vector intrinsics Serial sum\n");
#endif

//******************************************************

#ifdef HAVE_GCC_VECTOR_EXTENSIONS
      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_gcc_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   GCC vector intrinsics Serial sum\n");
#endif

//******************************************************

#ifdef HAVE_FOG_VECTOR_CLASS
      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_fog_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Fog C++ vector class Serial sum\n");
#endif

//******************************************************
      printf(" 4 wide vectors Kahan sum\n");
//******************************************************

#ifdef HAVE_X86_64_INTRINSICS_XXX
      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_intel_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Intel Vector intrinsics Kahan sum\n");
#endif

//******************************************************

#ifdef HAVE_GCC_VECTOR_EXTENSIONS
      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_gcc_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   GCC vector extensions Kahan sum\n");
#endif

//******************************************************

#ifdef HAVE_FOG_VECTOR_CLASS
      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_agner_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Fog C++ vector class Kahan sum\n");
#endif

//******************************************************
      printf(" 8 wide vector serial sum\n");
//******************************************************

#ifdef HAVE_AVX512
      cpu_timer_start(&cpu_timer);

      test_sum = do_sum(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Serial sum (OpenMP SIMD pragma)\n");
#endif

//******************************************************

#ifdef HAVE_X86_64_INTRINSICS_XXX
#ifdef HAVE_AVX512
      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_intel_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide Intel vector intrinsic Serial sum\n");
#endif
#endif

//******************************************************

#ifdef HAVE_GCC_VECTOR_EXTENSIONS
      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_gcc_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide GCC vector intrinsic Serial sum\n");
#endif

//******************************************************

#ifdef HAVE_FOG_VECTOR_CLASS
      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_fog_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide Fog C++ vector class Serial sum\n");
#endif

//******************************************************
      printf(" 8 wide vector Kahan sum\n");
//******************************************************

#ifdef HAVE_X86_64_INTRINSICS_XXX
#ifdef HAVE_AVX512
      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_intel_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide Intel Vector intrinsics Kahan sum\n");
#endif
#endif

//******************************************************

#ifdef HAVE_GCC_VECTOR_EXTENSIONS
      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_gcc_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide GCC vector extensions Kahan sum\n");
#endif

//******************************************************

#ifdef HAVE_FOG_VECTOR_CLASS
      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_agner_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide Fog C++ vector class Kahan sum\n");
#endif

//******************************************************
      free(energy);

      printf("\n");
   }
}

void cpu_timer_start(struct timespec *tstart_cpu){
   clock_gettime(CLOCK_MONOTONIC, tstart_cpu);
}

double cpu_timer_stop(struct timespec tstart_cpu){
   struct timespec tstop_cpu, tresult;
   clock_gettime(CLOCK_MONOTONIC, &tstop_cpu);
   tresult.tv_sec = tstop_cpu.tv_sec - tstart_cpu.tv_sec;
   tresult.tv_nsec = tstop_cpu.tv_nsec - tstart_cpu.tv_nsec;
   double result = (double)tresult.tv_sec + (double)tresult.tv_nsec*1.0e-9;

   return(result);
}
