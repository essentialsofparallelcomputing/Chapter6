CFLAGS=-fopenmp -std=c11 -g -O3 -ftree-vectorize -march=native -mtune=native -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info-loop-optimized -fopt-info-loop-missed
LDFLAGS=-fopenmp

default: globalsums assembler

globalsums: globalsums.o serial_sum.o kahan_sum.o kahan_intel_vector.o kahan_gcc_vector.o

assembler: serial_sum.asm kahan_sum.asm kahan_intel_vector.asm kahan_gcc_vector.asm

%.asm : %.o
	objdump -d -x86-asm-syntax=intel -no-show-raw-insn $< > $@
	#objdump -d -x86-asm-syntax=intel -no-show-raw-insn -S $< > $@

clean:
	rm -f globalsums *.o *.asm
