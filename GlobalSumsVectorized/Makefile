CFLAGS=-std=c11 -g -O3 -fstrict-aliasing -ftree-vectorize -fopenmp-simd -march=native -mtune=native -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info-loop-optimized -fopt-info-loop-missed
CFLAGS_QUIET=-std=c11 -g -O3 -fstrict-aliasing -ftree-vectorize -march=native -mtune=native -mprefer-vector-width=512
CFLAGS_NOVEC=-std=c11 -g -O3 -fstrict-aliasing -fno-tree-vectorize -march=native -mtune=native

CPPFLAGS=-Ivectorclass
CXXFLAGS=-g -O3 -fstrict-aliasing -ftree-vectorize -march=native -mtune=native -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info-loop-optimized -fopt-info-loop-missed -mprefer-vector-width=512 -DMAX_VECTOR_SIZE=512
LDFLAGS=-lm

default: globalsums #assembler

globalsums: globalsums.o serial_sum.o kahan_sum.o kahan_intel_vector.o kahan_gcc_vector.o kahan_fog_vector.o \
	        kahan_intel_vector8.o kahan_gcc_vector8.o kahan_fog_vector8.o serial_sum_intel_vector.o \
			serial_sum_gcc_vector.o serial_sum_intel_vector8.o serial_sum_gcc_vector8.o serial_sum_fog_vector.o serial_sum_fog_vector8.o \
	        serial_sum_novec.o

globalsums.o: globalsums.c
	$(CC) $(CPPFLAGS) $(CFLAGS_QUIET) -c $<

serial_sum_novec.o: serial_sum_novec.c
	$(CC) $(CPPFLAGS) $(CFLAGS_NOVEC) -c $<

assembler: serial_sum.asm kahan_sum.asm kahan_intel_vector.asm kahan_gcc_vector.asm kahan_fog_vector.asm \
	       kahan_intel_vector8.asm kahan_gcc_vector8.asm kahan_fog_vector8.asm serial_sum_intel_vector.asm serial_sum_intel_vector8.asm \
		   serial_sum_fog_vector.asm serial_sum_fog_vector8.asm

%.asm : %.o
	#objdump -d -x86-asm-syntax=intel -no-show-raw-insn $< > $@
	objdump -d -M=intel --no-show-raw-insn $< > $@

clean:
	rm -f globalsums *.o *.asm
