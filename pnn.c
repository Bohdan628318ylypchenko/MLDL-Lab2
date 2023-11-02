#include "pnn.h"

#include <stdlib.h>

void pnn_new(double sigma,
			 double a, double b, int segment_count,
			 pnn * nn,
			 double(*f)(double, double))
{
	int rfv_count = segment_count * segment_count;

	nn->sigma = sigma;
	nn->rfv_count = rfv_count;
	nn->refs = (v2 *)malloc(sizeof(v2) * rfv_count);
	nn->f_vals = (double *)malloc(sizeof(double) * rfv_count);

	double delta = (b - a) / (double)segment_count;
	double x, y;
	int k;
	for (int i = 0; i < segment_count; i++)
	{
		x = a + delta / 2.0 + (double)i * delta;
		for (int j = 0; j < segment_count; j++)
		{
			y = a + delta / 2.0 + (double)j * delta;

			k = i * segment_count + j;

			nn->refs[k].x = x;
			nn->refs[k].y = y;
			nn->f_vals[k] = f(x, y);
		}
	}
}
