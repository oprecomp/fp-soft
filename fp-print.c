/* Program to print multiple representation of a floating point number */

#include <stdio.h>
#include <stdlib.h>
#include "fp-soft-variable.h"

void printBin(long unsigned value, int len)
{
  if (len > 0)
    {
      printBin (value / 2, len - 1);
      putchar ((value & 1)?'1':'0');
    }
}
void printFloat(float_cast tmp, char *name)
{
  printf ("(f) %8.4f ",   tmp.f);
  printf ("    %08x ",   tmp.i);
  printf ("%c ",    (SIGN32(tmp.i)==0)?'+':'-');
  printf ("%2x ",   EXP32(tmp.i));
  printf ("%8lx ",  MANT32(tmp.i));
  printBin (EXP32(tmp.i),   8); putchar(' ');
  printBin (MANT32(tmp.i), 24); putchar(' ');
  printf ("%s ",    name);
  printf ("\n");
}

void printDouble(double_cast tmp, char *name)
{
  printf ("(d) %10.6f ", tmp.d);
  printf ("%016llx ",     tmp.i);
  printf ("%c ",         SIGN64(tmp.i)?'+':'-');
  printf ("%8lld ",      MANT64(tmp.i));
  printf ("%20ld ", (long int) MANT64(tmp.i));
  printf ("%16lx ", (long unsigned int) MANT64(tmp.i));
  printf ("%s ",    name);
  printf ("\n");
}

