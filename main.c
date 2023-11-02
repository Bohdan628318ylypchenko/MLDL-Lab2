#include "pnn.h"

#include <stdio.h>
#include <stdlib.h>

#define MDEBUG

#ifdef MDEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define EOI '\n'
#define INIT_LEN 16
#define INCR_COEF 2
#define MIN_LEFTOVER 1

#define USAGE_MSG "Usage:\n [n]ew sigma a b segment_count;\n [s]ave path;\n [l]oad path;\n [p]rint;\n [e]xit;\n [u]sage;\n"
#define COMMAND_PROMT "Command: "
#define ERR_FOPEN "Can't open file %s.\n"

static double f(double x, double y);
static char * finput_al(FILE * stream);

int main(void)
{
	char * input = NULL, * args = NULL;
	FILE * stream = NULL;

	pnn nn;
	nn.sigma = -1;
	nn.rfv_count = 0;
	nn.refs = NULL;
	nn.f_vals = NULL;

	puts(USAGE_MSG);
	while (1)
	{
		printf(COMMAND_PROMT);
		input = finput_al(stdin);

		switch (input[0])
		{
			case 'n':
				if (nn.refs != NULL)
					free(nn.refs);
				if (nn.f_vals != NULL)
					free(nn.f_vals);
				args = input + 2;
				double sigma, a, b;
				int segment_count;
				sscanf_s(args, "%lf %lf %lf %d", &sigma, &a, &b, &segment_count);
				pnn_new(sigma, a, b, segment_count,
						&nn,
						f);
				putchar('\n');
				break;
			case 's':
				args = input + 2;
				fopen_s(&stream, args, "w");
				if (stream == NULL)
				{
					fprintf(stderr, ERR_FOPEN, args);
					break;
				}
				pnn_fwrite(&nn, stream);
				fflush(stream);
				fclose(stream);
				putchar('\n');
				break;
			case 'l':
				if (nn.refs != NULL)
					free(nn.refs);
				if (nn.f_vals != NULL)
					free(nn.f_vals);
				args = input + 2;
				fopen_s(&stream, args, "r");
				if (stream == NULL)
				{
					fprintf(stderr, ERR_FOPEN, args);
					break;
				}
				pnn_fread(&nn, stream);
				fclose(stream);
				putchar('\n');
				break;
			case 'p':
				args = input + 2;
				int head_count;
				sscanf_s(args, "%d", &head_count);
				pnn_fprint(&nn, stdout, head_count);
				putchar('\n');
				break;
			case 'e':
				free(input);
				if (nn.refs != NULL)
					free(nn.refs);
				if (nn.f_vals != NULL)
					free(nn.f_vals);
				return EXIT_SUCCESS;
			case 'u':
				puts(USAGE_MSG);
				break;
		}

		free(input);
	}

	#ifdef MDEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	#endif

	return EXIT_SUCCESS;
}

static double f(double x, double y)
{
	return x + y;
}

static char * finput_al(FILE * stream)
{
	size_t buff_len = INIT_LEN, char_count = 0;
	char * buff = (char *)malloc(buff_len * sizeof(char));
	for (char c; (c = getc(stream)) != EOI;)
	{
		if (ferror(stream))
		{
			fputs("Error while reading input from stream.", stderr);
			abort();
		}

		if (buff_len - char_count <= MIN_LEFTOVER)
		{
			buff_len *= INCR_COEF;
			buff = (char *)realloc(buff, buff_len * sizeof(char));
		}

		buff[char_count++] = c;
	}

	buff[char_count] = '\0';

	return buff;
}