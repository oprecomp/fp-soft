/* Add 2 32 bits floating point numbers without multiplier 
*/

#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>

#include "fp-soft-variable.h"

int isZero (int Exp, __int64_t Mant)
{
  if ((0 == Exp) && (0 == (Mant & MANTISSA32_MASK)))
    return 1;
  else
    return 0;
}

__uint64_t fpaddparts(__uint64_t a, __uint64_t b)
{
  int precision, aSign, bSign, rSign, aExp, bExp, rExp, diffExp, absDiffExp;
  __int64_t rMant, aMant, bMant, mMant;

  aSign = SIGN32(a); aExp =  EXP32(a); aMant = MANT32(a);
  bSign = SIGN32(b); bExp =  EXP32(b); bMant = MANT32(b);
  diffExp = aExp - bExp;
  absDiffExp = abs (diffExp);
#if 0
  printf ("a: %X %7X %8lX\n", (aSign >> 31)&1, aExp, aMant);
  printf ("b: %X %7X %8lX\n", (bSign >> 31)&1, bExp, bMant);
  printf ("diff Exp: %d\n", diffExp);
  printFPBinMantissa(aMant);
  printFPBinMantissa(bMant);
#endif
  if (isZero (aExp, aMant))
      return b;
  if (isZero (bExp, bMant))
      return a;
  if (diffExp > 0)
    {
      bExp += absDiffExp;
      LSHIFT(bMant, absDiffExp);
    }
  else if (diffExp < 0)
    {
      aExp += absDiffExp;
      LSHIFT(aMant, absDiffExp);
    }
#if 0
  printFPBinMantissa(aMant);
  printFPBinMantissa(bMant);
  assert (aExp == bExp);
#endif
  if (aSign == bSign)
    {
      rSign = aSign;
      rExp  = aExp;
      rMant = aMant + bMant;
    }
  else // Differents signs
    {
      if (aSign) // Negative A
        {
          rMant = -aMant + bMant;
        }
      else // Negative B
        {
          rMant = aMant - bMant;
        }
      rExp = aExp;
      rSign = (rMant >= 0)?0:1;
      if (rMant < 0)
        {
          rMant = -rMant;
        }
      while (rMant < IMPLICIT32_1 && rMant)
        {
          rMant <<= 1;
          rExp--;
        }
    }
#if 0
  printFPBinMantissa(rMant);
#endif
  if (rMant >= (1 << (23+1)))
    {
      LSHIFT(rMant,1);
      rExp++;
    }
  if (0 == rMant)
    {
      rExp = 0;
    }
#if 0
  printFPBinMantissa(rMant);
  printf ("r: %X %7X %8lX\n", (rSign >> 31)&1, rExp, rMant);
#endif
  precision = fpGetPrecision();
  /* Precision emulation */
  mMant = ((1 << (precision + 1)) - 1) << (23 - precision);
  rMant = rMant & mMant;
  return PACK32(rSign, rExp, rMant);
}
__uint64_t addsf3_classical(__uint64_t a, __uint64_t b)
{
  __uint64_t result;

  fpIncadd();
  result = fpaddparts(a, b);
  return result;
}

float __addsf3(float a, float b)
{
  float_cast af, bf, cf;
  af.f = a;
  bf.f = b;
  cf.i = addsf3_classical(af.i, bf.i);
  return cf.f;
}

