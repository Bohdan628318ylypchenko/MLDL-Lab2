#include "pnn.h"

#include <stdlib.h>
#include <math.h>

static double act(v2 * x, v2 * ref, double sigma);
static double eucl2(v2 * a, v2 * b);

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
		x = a + (double)i * delta;
		for (int j = 0; j < segment_count; j++)
		{
			y = a + (double)j * delta;

			k = i * segment_count + j;

			nn->refs[k].x = x;
			nn->refs[k].y = y;
			nn->f_vals[k] = f(x, y);
		}
	}
}

double pnn_run(pnn * nn, v2 * x)
{
	double result = 0;

	for (int i = 0; i < nn->rfv_count; i++)
		result += act(x, &(nn->refs[i]), nn->sigma) * nn->f_vals[i];

	return result;
}

static double act(v2 * x, v2 * ref, double sigma)
{
	return exp((-1.0 * (eucl2(x, ref)) / (sigma * sigma)));
}

static double eucl2(v2 * a, v2 * b)
{
	double x = b->x - a->x;
	double y = b->y - a->y;
	return x * x + y * y;
}
