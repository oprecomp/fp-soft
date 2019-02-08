#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>
#include <math.h>

#include "fp-soft-variable.h"

void error(char * msg)
{
  printf ("Error %s\n", msg);
  printf ("./fp-main-test p [FPNumber]+\n");
  printf ("./fp-main-test [+|*] FPNumber1 FPNumber2 Precision [Result]\n");
  exit(1);
}

void printPrecision()
{
  int i;
  printf ("Prec.  :");
  for (i = 0; i < 24; i++)   printf ("%4d ", i);
  printf ("\nFmul32 :");
  for (i = 0; i < 24; i++)   printf ("%4lld ", fpGetmulCount(i));
  printf ("\nFadd32 :");
  for (i = 0; i < 24; i++)   printf ("%4lld ", fpGetaddCount(i));
  printf ("\nFsub32 :");
  for (i = 0; i < 24; i++)   printf ("%4lld ", fpGetsubCount(i));
  printf ("\n");
}

int main (int argc, char* argv[])
{
  float_cast a, b, c;
  double_cast d;
  int i, tmp;

  printf ("float_cast %u float_cast %u \n", sizeof (float_cast), sizeof (double_cast));
  assert ((32/8) == sizeof (float_cast));
  assert ((64/8) == sizeof (double_cast));

  if (argc < 2)
      error ("Missing argument");
  if (argv[1][0] == 'p')
    {
      for (i = 2; i < argc; i++)
        {
          a.f = (float) atof (argv[i]);
          d.d =  atof (argv[i]);
          PrF(a);
          PrD(d);
        }
    }
  else                          /* Operation */
    {
      if (argc < 5)
        error ("Missing arguments");
      a.f = (float) atof (argv[2]);
      b.f = (float) atof (argv[3]);
      c.i = 0;
      tmp = atoi(argv[4]);
      if ((tmp < 0) || (tmp > 23))
        error ("Incorrect precision 0 < p < 24");
      fpInitCount();
      fpSetPrecision (tmp);
#define PF(V)   printFloat  (V, #V)
      PrF(a);
      PrF(b);
      PrF(c);
      switch (argv[1][0])
        {
        case '*':
          c.f = a.f * b.f;
          PF(c);
          c.i = mulsf3_classical (a.i, b.i);
          PF(c);
          break;
        case '+':
          c.f = a.f + b.f;
          PF(c);
          c.i = addsf3_classical (a.i, b.i);
          PF(c);
          break;
        case '-':
          c.f = a.f - b.f;
          PF(c);
          b.i ^= LONG32_SIGNBIT_MASK;
          c.i = subsf3_classical (a.i, b.i);
          PF(c);
          break;
        case '/':
          error ("/ not yet implemented");
          break;
        default:
          error ("Unknown command");
          break;
        }
      if (argc > 5)
        {
          float result, diff;
          result = atof (argv[5]);
          diff = fabs(result - c.f);
          printf ("%f %f %f\n", c.f, result, diff);
          if (diff < 1e-6)
            return 0;
          else
            return 1;
        }
    }
  printPrecision();
  return 0;
}

