CFLAGS=-std=c11 -g -O3 -fstrict-aliasing -ftree-vectorize -march=native -mtune=native -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info-loop-optimized -fopt-info-loop-missed
CFLAGS_QUIET=-std=c11 -g -O3 -fstrict-aliasing -ftree-vectorize -march=native -mtune=native

default: globalsums assembler

globalsums: globalsums.o serial_sum.o kahan_sum.o kahan_intel_vector.o kahan_gcc_vector.o

globalsums.o: globalsums.c
	$(CC) $(CPPFLAGS) $(CFLAGS_QUIET) -c $<

assembler: serial_sum.asm kahan_sum.asm kahan_intel_vector.asm kahan_gcc_vector.asm

%.asm : %.o
	objdump -d -x86-asm-syntax=intel -no-show-raw-insn $< > $@

clean:
	rm -f globalsums *.o *.asm
