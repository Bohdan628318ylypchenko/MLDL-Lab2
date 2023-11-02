#include "pnn.h"

#include <stdio.h>
#include <stdlib.h>

void pnn_fread(pnn * nn, FILE * stream)
{
	fread(&(nn->sigma), sizeof(double), 1, stream);

	int rfv_count;
	fread(&rfv_count, sizeof(int), 1, stream);
	nn->rfv_count = rfv_count;

	nn->refs = (v2 *)malloc(sizeof(v2) * rfv_count);
	nn->f_vals = (double *)malloc(sizeof(double) * rfv_count);
	fread(nn->refs, sizeof(v2), rfv_count, stream);
	fread(nn->f_vals, sizeof(double), rfv_count, stream);
}

void pnn_fwrite(pnn * nn, FILE * stream)
{
	fwrite(&(nn->sigma), sizeof(double), 1, stream);
	fwrite(&(nn->rfv_count), sizeof(int), 1, stream);

	fwrite(nn->refs, sizeof(v2), nn->rfv_count, stream);
	fwrite(nn->f_vals, sizeof(double), nn->rfv_count, stream);
}

void pnn_fprint(pnn * nn, FILE * stream, int head_count)
{
	fprintf(stream, "sigma = %lf; rfv_count = %d\n", nn->sigma, nn->rfv_count);

	for (int i = 0; (i < nn->rfv_count) && (i < head_count); i++)
		fprintf(stream, "ref #%d: x = %lf, y = %lf | f value #%d: %lf\n",
				i, nn->refs[i].x, nn->refs[i].y, i, nn->f_vals[i]);
}
