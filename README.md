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
  * git clone --recursive https://github.com/riscv/riscv-gnu-toolchain
  * ./configure --prefix=/opt/riscv --with-arch=rv32i --with-abi=ilp32; make
  * cd qemu; ./configure --prefix=/opt/riscv --target-list=riscv32-linux-user; make

# How to compile
* "make riscv" will compile the library for RISCV platform. Adjust RVCC variable for your installation. Compilation will produce a libfp-soft.so file
  * "make riscv-build-example" will compile for RISCV platform a matrix multiply example. 
  * "make riscv-run-example should give the following result: 
``` 96.000000 80.000000 80.000000 128.000000 
 112.000000 56.000000 80.000000 112.000000 
 64.000000 28.000000 56.000000 64.000000 
 80.000000 80.000000 80.000000 112.000000 

 133.273682 111.862587 131.518982 189.546661 
 147.577988 89.743591 116.597992 164.783981 
 94.794693 41.236095 74.267197 91.267288 
 109.496696 99.617393 110.356194 162.936279 

 Prec.  :   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23 
 Fmul32 :   0    0   64    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0   64 
 Fadd32 :   0    0   64    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0   64 
 Fsub32 :   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0 
```

  These results are the result of the multiplication of the very same matrices but with (1) 2 bits mantisa and (2) 23 bits mantissa. The last array give the number for mul / add / sub with the different precision.

* "make x86" will compile the library & the '''fp-main-test''' main application which can 
  * help to figure out what are the internal representation of different numbers. 
  * It can also serve as debugging application for the library.

# How to use
* "make riscv-run-example" will compile and execute a matrix multiplication example, using qemu-risc32 simulator (correct your shell PATH variable accordingly)

# References:
    
* Floats Formats :
  *  FP Double : Sign bit: 1 Exponent: 11 bits  Significand precision: 52 bits 
  *  FP Float  : Sign bit: 1 Exponent:  8 bits  Significand precision: 23 bits 
  *  https://en.wikipedia.org/wiki/Double-precision_floating-point_format
  *  https://en.wikipedia.org/wiki/Single-precision_floating-point_format
  *  https://en.wikipedia.org/wiki/Exponent_bias

* Berkeley SoftFloat is a free, high-quality software implementation  of binary floating-point that conforms to the IEEE Standard for Floating-Point Arithmetic
 * http://www.jhauser.us/arithmetic/SoftFloat.html

* Used in RISCV boot : https://github.com/riscv/riscv-pk

* GCC gcc/libgcc/fp-bit.[ch] fpmul_parts function

* There is certainly an LLVM implementation somewhere, let me know where ?
