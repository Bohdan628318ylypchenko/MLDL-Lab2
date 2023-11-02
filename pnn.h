#pragma once

#include <stdio.h>

typedef struct
{
	double x;
	double y;
} v2;

typedef struct
{
	double sigma;
	int rfv_count;
	v2 * refs;
	double * f_vals;
} pnn;

void pnn_new(double sigma,
			 double a, double b, int segment_count,
			 pnn * nn,
			 double(*f)(double, double));

void pnn_fread(pnn * nn, FILE * stream);

void pnn_fwrite(pnn * nn, FILE * stream);

void pnn_fprint(pnn * nn, FILE * stream);
