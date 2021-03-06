X86CC=gcc
X86CFLAGS = -Wall -g
X86AR =ar

RVROOT = /opt/riscv/
RVCFLAGS = -Wall -g -march=rv32i -mabi=ilp32
RVCC=${RVROOT}/bin/riscv32-unknown-elf-gcc 
RVAR=${RVROOT}/bin/riscv32-unknown-elf-ar
RVEMU=${RVROOT}/bin/qemu-riscv32
# CC = gcc

FILES=fp-mul32.o fp-add32.o fp-sub32.o fp-precision.o fp-print.o
FILESTEST = fp-main-test.o 

.c.o:
	${CC} ${CFLAGS} -c $^

all: 
	echo "x86 or riscv ?"

x86:
	${MAKE} CC="${X86CC}" CFLAGS="${X86CFLAGS}" AR="${X86AR}" clean libfp-soft.so fp-main-test 

riscv:
	${MAKE} CC="${RVCC}" CFLAGS="${RVCFLAGS}" AR="${RVAR}" clean libfp-soft.so fp-main-test

riscv-build-example:
	make clean riscv
	${RVCC} ${CFLAGS} -o Example-Matrix-Multiply Example-Matrix-Multiply.c libfp-soft.so 

riscv-run-example:
	make riscv-build-example
	${RVEMU} Example-Matrix-Multiply

fp-main-test: fp-main-test.o ${FILES} fp-soft-variable.h
	${CC} ${CFLAGS} -o fp-main-test fp-main-test.o libfp-soft.so 

libfp-soft.so: ${FILES} fp-soft-variable.h
	${AR} rcs libfp-soft.so ${FILES}

clean:
	-rm ${FILES}
	-rm fp-main-test fp-main-test.o libfp-soft.so 
