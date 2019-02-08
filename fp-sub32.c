/* Sub 2 32 bits floating point numbers without multiplier 
*/

#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>

#include "fp-soft-variable.h"

__uint64_t subsf3_classical(__uint64_t a, __uint64_t b)
{
  __uint64_t result;

  fpIncsub();
  result = fpaddparts(a, b);
  return result;
}

float __subsf3(float a, float b)
{
  float_cast af, bf, cf;
  af.f = a;
  bf.f = b;
  bf.i ^= LONG32_SIGNBIT_MASK;
  cf.i = subsf3_classical(af.i, bf.i);
  return cf.f;
}

