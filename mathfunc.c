/*
 *	TABULA VIGILANS: mathematical functions
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "tv.h"
#include "rules.h"
#include "xtab.h"

int prog_argc;
char **prog_argv;

double
mf_abs(double val)
{
	return fabs(val);
}

double
mf_argc(void)
{
	return (double)prog_argc;
}

double
mf_argv(double idx)
{
	int i = (int)(idx + 0.5)-1;

	return (i < 0 || i >= prog_argc) ? 0.0 : atof(prog_argv[i]);
}

char *
mf_args(double idx)
{
	int i = (int)(idx + 0.5)-1;

	return (i < 0 || i >= prog_argc) ? "" : prog_argv[i];
}

double
mf_dimensions(Table *tab)
{
	if(tab == 0) {
		fprintf(stderr, "TV: attempt made to find number of dimensions of null table variable\n");
		tidy_up(7);
	}
	return (double)tab->tabdim.dimensions;
}

double
mf_dimsize(Table *tab, double fdim)
{
	int dim = (int)(fdim-1);
	char	str[60];

	if(tab == 0) {
		fprintf(stderr,
	"TV: attempt made to find dimension-size of null table variable\n");
		tidy_up(7);
	}
	if(dim < 0 || dim >= tab->tabdim.dimensions) {
		sprintf(str,
			"TV: table does not have dimension %d\n", dim+1);
		fprintf(stderr, str);
		tidy_up(7);
	}
	return (double)tab->tabdim.sizes[dim];
}

double
mf_gamma(void)
{
	int i;
	int limit = (1 + (int)(drand48() * 10.0))*2;
	double result, sum = 1.0;

	for(i = 0; i < limit; i++)
		sum *= drand48();
	result = -log(sum)/(limit * 3.0);
	return (result > 1.0 ? 1.0 : result);
}

double
mf_gauss(void)
{
	int i, num = 12;
	double result, sum = 0.0,  halfnum = num/2.0;

	for(i = 0; i < num; i++)
	    sum += drand48();
	result = ((sum - halfnum) + 3.0)/halfnum;
	result = (result < 0.0 ? 0.0 : result);
	return(result > 1.0 ? 1.0: result);
}

double
mf_power(double in1, double in2)
{
	double	frac;		/* fractional part of power arg */

	if(in1 < 0)
	{
	    frac = fmod(in1, 1.0);
	    if(frac)
	    {
		return 0.0;
	    }
	}
	return pow(in1, in2);
}

double
mf_rand(void)
{
	return drand48();
}

double
mf_random(double low, double high)
{
	return low + drand48()*(high-low);
}

double
mf_round(double d)
{
	return floor(d+0.5);
}

double
mf_sine(double input)
{
	return sin(input);
}

/* New maths functions */
double
mf_cosine(double input)
{
	return cos(input);
}

double
mf_tangent(double input)
{
	return tan(input);
}

double
mf_arcsine(double input)
{
	return asin(input);
}

double
mf_arccosine(double input)
{
	return acos(input);
}

double
mf_arctangent(double input)
{
	return atan(input);
}

double
mf_natlog(double input)		/* RO added 5/11/05 */
{
	return log(fabs(input)); /* log will not accept negative input! */
}

double
mf_log10(double input)		/* RO added 10/11/05 */
{
	if(input == 0.0) {
		return 0;
	}
	return log10(fabs(input));
}
/* end of new */

double
mf_sqrt(double input)
{
	return sqrt(input);
}

#ifdef _WIN32
double
drand48(void)
{
	return (double)rand()/(double)RAND_MAX;
}
#endif
