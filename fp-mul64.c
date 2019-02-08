#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>
#include <stdint.h>
#include "fp-soft-variable.h"
typedef union {					/* Multiple view for a 64 bits FP number */
  double d;
  __uint64_t i;

    struct
  {
    __uint64_t m : 52;
    int x : 11;
    unsigned int s : 1;
  } p;
} double_cast;

/* MANTISSA_BITS aka 53 is the "normal" value for precision */
static unsigned int mulsd3_precision = 52;

/* Set the iteration number in the fpmulparts algorithm  */
void fpmulSetPrecision (int mantissa_bits)
{
  mulsd3_precision = mantissa_bits;
}

static long long fpmul_count[53];    /* Count fp multiplications per precision */

void fpmulInitFmulPrecision (int mantissa_bits)
{
  mulsd3_precision = mantissa_bits;
}

void fpmulInitFpmulCount()
{
  bzero(fpmul_count, 53*sizeof (long long));
}

long long fpmulGetFmulCount (int index)
{
  return fpmul_count[index];
}

__uint64_t fpmulparts (uint64_t a0,uint64_t a32, uint64_t b32, uint64_t b0)
{
  __uint64_t mult = (__uint64_t)a0;
  //__uint64_t mult = (__uint64_t)a32;
  __uint64_t total = 0;
  __uint64_t ret;
  int bit, diff;

  diff = MANTISSA_BITS - mulsd3_precision;
  mult <<= diff;
  a0 >>= diff;
//a0
  for (bit = diff; bit < MANTISSA_BITS; bit++)
    {
      if (a0 & 0x1)
        {
          total += mult;
        }
      mult <<= 1;
      a0 >>= 1;
    }
  ret = total>>SHORT_SHIFT;
//32
  a32 >>= diff;
  for (bit = diff; bit < MANTISSA_BITS; bit++)
    {
      if (a32 & 0x1)
        {
          total += mult;
        }
      mult <<= 1;
      a32 >>= 1;
    }
  ret = total>>SHORT_SHIFT;
//b0
  b0 >>= diff;
  for (bit = diff; bit < MANTISSA_BITS; bit++)
    {
      if (b0 & 0x1)
        {
          total += mult;
        }
      mult <<= 1;
      b0 >>= 1;
    }
  ret = total>>SHORT_SHIFT;
// b32
  b32 >>= diff;
  for (bit = diff; bit < MANTISSA_BITS; bit++)
    {
      if (b32 & 0x1)
        {
          total += mult;
        }
      mult <<= 1;
      b32 >>= 1;
    }
  ret = total>>SHORT_SHIFT;
  return ret;
}

__uint64_t mulsd3_classical(a1,a2)
{
    register __uint64_t result;
    register int sign;
    register int exp;
    register int tmp;

    //calculate the sign
    sign = ((a1>>SHORT_SHIFT^a2>>SHORT_SHIFT))&SHORT_SIGNBIT_MASK;
    //take the exponent for each operand
    exp = SHORT_EXPONENT_MASK & (a0>>SHORT_SHIFT);
    tmp = SHORT_EXPONENT_MASK & (a32>>SHORT_SHIFT);

    if(!a1 || !a2)
        return 0;
    a1 = MANT(a1);
    a2 = MANT(a2);
    exp -= EXCESS<<EXPONENT_SHIFT;
    exp += tmp;
    result = fpmulparts(a1,a2);
    //normalize and round the result
    if (result & LONG_SIGNBIT_MASK)
    {
        /* round */
        result += 0x80ul;
        result >>= (EXPONENT_SHIFT+1);
    }
    else
    {
        result += 0x40ul;
        result >>= EXPONENT_SHIFT;
        exp -= 1<<EXPONENT_SHIFT;
    }
    if (result & (HIDDEN<<1))
    {
        result >>= 1;
        exp += 1<<EXPONENT_SHIFT;
    }
    result &= ~HIDDEN;
    //put all the parts together again
    result |= ((long)(sign | exp)<<SHORT_SHIFT);
    return result;
}

#ifdef WITH_GCCWRAP
 double __wrap___mulsd3(double a, double b)
#else
 double __mulsd3(double a, double b)
#endif
{
  double_cast ad, bd, cd;
  printf("%15e %15e\n", a,b);
  fpmul_count[mulsd3_precision]++;
  ad.d = a;
  bd.d= b;
  cd.i = mulsd3_classical(a0.i, a32.i, b0.i,b32.i);
  return cd.d;
}

#ifdef WITH_MAIN
void printDouble(double_cast tmp, char *name)
{
  printf ("(d) %10.6e ", tmp.d);
  printf ("        %08x ", tmp.i);
  printf ("%c ",    (tmp.p.s==0)?'+':'-');
  printf ("%8d ",   tmp.p.x);
  printf ("%8d ",   tmp.p.x+127);
  printf ("%20d ",  tmp.p.m);
  printf ("%16x ",  tmp.p.m);
  printf ("%s ",    name);
  printf ("\n");
}
void printDouble(double_cast tmp, char *name)
{
  printf ("(d) %10.6e ", tmp.d);
  printf ("%016lx ", tmp.i);
  printf ("%c ",    (tmp.p.s==0)?'+':'-');
  printf ("%8d ",   tmp.p.x);
  printf ("%8d ",   tmp.p.x+1023);
  printf ("%20ld ", tmp.p.m);
  printf ("%16lx ", tmp.p.m);
  printf ("%s ",    name);
  printf ("\n");
}

int main (int argc, char* argv[])
{
  double_cast a0, a32, b0, b32, c;
  int result;
  float cr;

  printf ("double_cast %d double_cast %d \n", sizeof (double_cast), sizeof (double_cast));
  assert ((64/8) == sizeof (double_cast));
  assert ((64/8) == sizeof (double_cast));
#if 1
  if (argc < 5)
    {
      printf ("%s FPNumber1 FPNumber2 precision\n", argv[0]);
      exit(0);
    }
  a0.d = (double) atof (argv[1]);
  a32.d = (double) atof (argv[2]);
  b0.d = (double) atof (argv[3]);
  b32.d = (double) atof (argv[4]);
  fpmulSetPrecision (atoi(argv[5]));
#else
  a0.f = (float) 3.0;
  a32.f = (float) 3.0;
  b0.f = (float) 4.0;
  b32.f = (float) 5.0;
#endif  /* 1 */
  c.i = 0;
  printf ("Computing mantisa %d\n", mulsd3_precision);
#define PD(V);  printFloat  (V, #V)
  PD(a0);
  PD(a32);
  PD(b0);
  PD(b32);
  PD(c);
  c.i = mulsd3_classical (a0.i,a32.i, b0.i,b32.i);
  PD(c);
  c.d = a0.d * b0.d; a32.d * b32.d
  PD(c);
  return 0;
}
#endif  /* WITH_MAIN */
