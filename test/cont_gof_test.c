#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../src/distrs.h"
#include "../src/utils.h"

/*
 * Critical value of the distribution of the test statistic in the Cramer-von
 * Mises test for large samples, with 99% significance.
 */
#define CRITICAL_VAL 2.3263

// sample size
#define N 100000

double sample[N];

int double_comp(const void *a, const void *b)
{
    return *((double*) b) - *((double*) a);
}

int cramer_von_mises(double (*sampler)(), double (*cdf)(double), char *name)
{
    for(size_t i=0; i < N; sample[i++] = sampler());
    qsort(sample, N, sizeof(double), double_comp);
    double statistic = 1.0 / 12 / N;
    for(size_t i=1; i <= N; i++)
        statistic += square(cdf(sample[i-1]) - (2.0*i - 1)/2/N);
    if(statistic < CRITICAL_VAL) {
        printf("%s passed Cramer-von Mises (statistic=%f).\n", name, statistic);
        return 1;
    }
    printf("%s failed Cramer-von Mises (statistic=%f).\n", name, statistic);
    return 0;
}

double sampler_1() { return uniform(0, 1); }
double cdf_1(double x) { return uniform_cdf(x, 0, 1); }

int main()
{
    return !cramer_von_mises(sampler_1, cdf_1, "uniform(0, 1)");
}
