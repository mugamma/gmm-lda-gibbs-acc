#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "../src/distrs.h"
#include "../src/utils.h"

/*
 * Critical value of the distribution of the test statistic in the Cramer-von
 * Mises test for large samples, with 99% significance.
 */
#define CRITICAL_VAL 2.3263

// sample size
#define N 100000

float sample[N];

int float_comp(const void *a, const void *b)
{
    return *((float*) a) - *((float*) b) < 0 ? -1 : 1;
}

int cramer_von_mises(float (*sampler)(), float (*cdf)(float), char *name)
{
    for(size_t i=0; i < N; sample[i++] = sampler());
    qsort(sample, N, sizeof(float), float_comp);
    float statistic = 1.0 / 12 / N;
    for(size_t i=1; i <= N; i++)
        statistic += square(cdf(sample[i-1]) - (2.0*i - 1)/2/N);
    int success = statistic < CRITICAL_VAL ? 1 : 0;
    printf("%s %s Cramer-von Mises (statistic=%f).\n", name,
           success ? "passed" : "failed", statistic);
    return success;
}

float sampler_1() { return uniform(0, 1); }
float cdf_1(float x) { return uniform_cdf(x, 0, 1); }

float sampler_2() { return gaussian(0, 1); }
float cdf_2(float x) { return gaussian_cdf(x, 0, 1); }

float sampler_3() { return gaussian(2, 0.4); }
float cdf_3(float x) { return gaussian_cdf(x, 2, 0.4); }

float sampler_4() { return gamma(1, 1); }
float cdf_4(float x) { return gamma_cdf(x, 1, 1); }

float sampler_5() { return gamma(1.25, 2); }
float cdf_5(float x) { return gamma_cdf(x, 1.25, 2); }

float sampler_6() { return inverse_gamma(1, 1); }
float cdf_6(float x) { return inverse_gamma_cdf(x, 1, 1); }

float sampler_7() { return inverse_gamma(1.25, 2); }
float cdf_7(float x) { return inverse_gamma_cdf(x, 1.25, 2); }

int main()
{
    srand(time(NULL));
    return !(cramer_von_mises(sampler_1, cdf_1, "uniform(0, 1)") &&
             cramer_von_mises(sampler_2, cdf_2, "gaussian(0, 1)") &&
             cramer_von_mises(sampler_3, cdf_3, "gaussian(2, 0.4)") &&
             cramer_von_mises(sampler_4, cdf_4, "gamma(1, 1)") &&
             cramer_von_mises(sampler_5, cdf_5, "gamma(1.25, 2)") &&
             cramer_von_mises(sampler_6, cdf_6, "inverse_gamma(1, 1)") &&
             cramer_von_mises(sampler_7, cdf_7, "inverse_gamma(1.25, 2)"));
}
