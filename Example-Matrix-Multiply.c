#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "fp-soft-variable.h"
#define NLINE 4
#define NCOL 4
typedef float typeelt;
typedef typeelt tMatrix[NLINE][NCOL];

char printfFormat[] = "%08f ";

void printMatrix(tMatrix a)
{
	int line, col;
	for (line = 0; line < NLINE; line++)
	{
		for (col = 0; col < NCOL; col++)
			printf(printfFormat, a[line][col]);
		printf("\n");
	}
	printf("\n");
}

void cleanMatrix(tMatrix a)
{
	int line, col;
	for (line = 0; line < NLINE; line++)
		for (col = 0; col < NCOL; col++)
			a[line][col] = 0;
}

void sumMatrix(tMatrix a, tMatrix b, tMatrix res)
{
	int line, col;
	for (line = 0; line < NLINE; line++)
		for (col = 0; col < NCOL; col++)
			res[line][col] = a[line][col] + b[line][col];
}

void mulMatrix(tMatrix a, tMatrix b, tMatrix res)
{
	int line, col, k;
	for (line = 0; line < NLINE; line++)
		for (col = 0; col < NCOL; col++)
		{
			res[line][col] = 0;
			for (k = 0; k < NCOL; k++)
				res[line][col] += a[line][k] * b[k][col];
		}
}

void diagMatrix(tMatrix a, typeelt value)
{
	int indice, line;
	indice = (NLINE < NCOL)?NLINE:NCOL;
	for (line = 0; line < indice; line++)
		a[line][line] = value;
}

void firstLineMatrix(tMatrix a, typeelt value)
{
	int col;
	for (col = 0; col < NCOL; col++)
		a[0][col] = value;
}

void firstColMatrix(tMatrix a, typeelt value)
{
	int line;
	for (line = 0; line < NCOL; line++)
		a[line][0] = value;
}

void randMatrix(tMatrix a)
{
	int line, col;
	for (line = 0; line < NLINE; line++)
		for (col = 0; col < NCOL; col++)
			a[line][col] = (typeelt) (rand () % 1000)/100.0;
}

int main(int argc, char * argv[])
{
	tMatrix a, b, c;
	int i;
	cleanMatrix(a);  cleanMatrix(b);   cleanMatrix(c);
	randMatrix(a);  randMatrix(b);

    fpSetPrecision (2);
    mulMatrix(a, b, c);
	printMatrix (c);

    fpSetPrecision (23);
    mulMatrix(a, b, c);
	printMatrix (c);


    printf ("Prec.  :");
    for (i = 0; i < 24; i++)   printf ("%4d ", i);
    printf ("\nFmul32 :");
    for (i = 0; i < 24; i++)   printf ("%4lld ", fpGetmulCount(i));
    printf ("\nFadd32 :");
    for (i = 0; i < 24; i++)   printf ("%4lld ", fpGetaddCount(i));
    printf ("\nFsub32 :");
    for (i = 0; i < 24; i++)   printf ("%4lld ", fpGetsubCount(i));
    printf ("\n");
    
	return 0;
}
