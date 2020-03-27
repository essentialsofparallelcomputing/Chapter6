# Chapter 6 Vectorization: FLOPs for free
This is from Chapter 6 of Parallel and High Performance Computing, Robey and Zamora,
Manning Publications, available at http://manning.com

The book may be obtained at
   http://www.manning.com/?a_aid=ParallelComputingRobey

Copyright 2019 Robert Robey, Yuliana Zamora, and Manning Publications
Emails: brobey@earthlink.net, yzamora215@gmail.com

See License.txt for licensing information.

Examining autovectorization
   Requirements: vectorizing compiler such as GCC or Intel

   autovec (Book: listing 6.1)
      Build with cmake, make
      Observe the output from your compiler and whether it vectorizes the loops. For GCC 8.2
         stream_triad.c:22:7: optimized: loop vectorized
         stream_triad.c:14:4: optimized: loop vectorized
      Run the program ./stream_triad
         Average runtime is 0.141520 msecs

   autovec_function (Book: listing 6.2)
      Build with cmake, make
      Observe the output from your compiler and whether it vectorizes the loops. For GCC 8.2
         stream_triad.c:10:4: optimized: loop vectorized
         stream_triad.c:10:4: optimized:  loop versioned for vectorization because of possible aliasing
         stream_triad.c:10:4: optimized: loop vectorized
         stream_triad.c:18:4: optimized: loop vectorized
      Run the program ./stream_triad
         Average runtime is 0.137134 msecs
      
   autovec_restrict (Book: modified version of listing 6.2 with restrict)
      Build with cmake, make
      Observe the output from your compiler and whether it vectorizes the loops. Note whether the vectorization
            report is different than autovec_function. For GCC 8.2
         stream_triad.c:10:4: optimized: loop vectorized
         stream_triad.c:10:4: optimized: loop vectorized
         stream_triad.c:18:4: optimized
      Run the program ./stream_triad
         Average runtime is 0.138037 msecs

Exploring compiler hints
   timestep (Book: listing 6.3)
      Build with cmake, make
      Observe the output from your compiler and whether it vectorizes the loops.
         For GCC 8.2
            timestep.c:9:4: missed: couldn't vectorize loop
            timestep.c:9:4: missed: not vectorized: control flow in loop
      Try various optimized versions (timestep.c.opt-opt4)
         cp timestep.c.opt timestep.c and recompile
      Observe for which versions your compiler vectorizes the timestep loop. It is not clear from
         the GCC output if it vectorizes the loop for these. Intel will vectorize,
         For GCC 8.2
            timestep.c:9:9: optimized: loop vectorized
            timestep.c:11:7: missed: couldn't vectorize loop
            timestep.c:11:7: missed: not vectorized: control flow in loop.
         Run with likwid-perfctr shows that no vectorized instructions occur (need linux system
            with likwid tool installed)
      Run the program ./timestep
         Minimum dt is 0.016964
   
   mass_sum (Book: listing 6.4)
      Build with cmake, make
      Observe the output from your compiler and whether it vectorizes the loops.
         For GCC 8.2 -- similar to timestep
      Run the program ./mass_sum

   stencil (Book: listing 6.5)
      Build with cmake, make
      Check the vectorization report
         For GCC, the version with the OpenMP SIMD pragma vectorizes the loop and generates multiple
            versions for the other two
         For Intel, the version with the OpenMP SIMD pragma and the restrict modifier both vectorize and
            generates multiple versions if neither are there.

Intrinsics
   GlobalSumsVectorized (Book: listings 6.6 through 6.8)
      Build with cmake, make
      Run with ./globalsums
      Output shows the performance of the different test versions of the global sum
