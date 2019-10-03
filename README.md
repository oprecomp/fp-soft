This directory contain a small floating point library replacement used
for transprecision experimentation. The main characteristics are :

* compatible with float ___without any source___ modifications
* small api to modify precisions at run-time
* ___NOT IEEE compliant___, only for research purpose
* Implement *, +, - for float 32 

# The abi is very simple :
~~~~
    float    __mulsf3(float a, float b);
    float    __addsf3(float a, float b);
    float    __subsf3(float a, float b);
    void fpSetPrecision (int mantissa_bits);
    void fpInitCount();
    int fpGetPrecision();
~~~~
# How to install
Use a riscv toolchain containing gcc and qemu like
* the RISCV gnu toolchain https://github.com/riscv/riscv-gnu-toolchain , using :
** git clone --recursive https://github.com/riscv/riscv-gnu-toolchain
** ./configure --prefix=/opt/riscv --with-arch=rv32i --with-abi=ilp32; make
** cd qemu; ./configure --prefix=/opt/riscv ; make
# How to compile
* "make riscv" will compile the library for RISCV platform. Adjust RVCC variable for your installation. Compilation will produce a libfp-soft.so file
* "make riscv-build-example" will compile for RISCV platform a matrix multiply example. 

# How to use
* "make riscv-run-example" will compile and execute a matrix multiplication example, using qemu-risc32 simulator

# References:
    
* Floats Formats :
**  FP Double : Sign bit: 1 Exponent: 11 bits  Significand precision: 52 bits 
**  FP Float  : Sign bit: 1 Exponent:  8 bits  Significand precision: 23 bits 
**  https://en.wikipedia.org/wiki/Double-precision_floating-point_format
**  https://en.wikipedia.org/wiki/Single-precision_floating-point_format
**  https://en.wikipedia.org/wiki/Exponent_bias

* Berkeley SoftFloat is a free, high-quality software implementation
  of binary floating-point that conforms to the IEEE Standard for
  Floating-Point Arithmetic
  http://www.jhauser.us/arithmetic/SoftFloat.html

* Used in RISCV boot : https://github.com/riscv/riscv-pk

* GCC gcc/libgcc/fp-bit.[ch] fpmul_parts function

* LLVM implementation ?
