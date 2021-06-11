#include <stdlib.h>
#include <float.h>

#include <math.h>

#ifndef M_E
# define M_E		2.7182818284590452354	/* e */
#endif
#ifndef M_PI
# define M_PI		3.14159265358979323846	/* pi */
#endif

#include "utils.h"
#include "distrs.h"


float uniform(float a, float b)
{
    return a + rand() * (b - a) / RAND_MAX;
}

float uniform_pdf(float x, float a, float b)
{
    return x < b && x > a ? 1/(b - a) : 0;
}

float uniform_cdf(float x, float a, float b)
{
    return x < a ? 0 : x < b ? (x - a)/(b - a) : 1;
}

int categorical(float *param, size_t n)
{
    float u = uniform(0, 1), sum=0;
    int i = 0;
    while(u > sum)
        sum += param[i++];
    return i - 1;
}

float categorical_pdf(int x, float *param, size_t n)
{
    if(x < 0 || x >= n)
        return 0;
    return param[x];
}

float categorical_cdf(int x, float *param, size_t n)
{
    if(x < 0 || x >= n)
        return 0;
    float res = param[0];
    while(x)
        res += param[x--];
    return res;
}

float gaussian(float mean, float var)
{
    float u=uniform(0, 1), v=uniform(0, 1);
    return mean + sqrt(-2 * log(u) * var) * cos(2*M_PI * v);
}

float gaussian_pdf(float x, float mean, float var)
{
    return exp(-square(x - mean)/2/var) / sqrt(2*M_PI * var);
}

float gaussian_cdf(float x, float mean, float var)
{
    return (1 + erf((x - mean) / sqrt(2 * var))) / 2;
}

//Wikipedia algorithm
float gamma(float shape, float rate)
{
    int n = floor(shape);
    float delta = shape - n, exp_part = 0,  xi, eta;
    while(n--)
        exp_part += -log(uniform(0, 1));
    if(delta > DBL_EPSILON) do {
        if(uniform(0, 1) < M_E / (M_E + delta)) {
            xi = pow(uniform(0, 1), 1/delta);
            eta = uniform(0, 1) * pow(xi, delta - 1);
        } else {
            xi = 1 - log(uniform(0, 1));
            eta = uniform(0, 1) * exp(-xi);
        }
    } while(eta > pow(xi, delta -1) * exp(-xi));
    return (xi + exp_part) /rate;
}

float gamma_pdf(float x, float shape, float rate)
{
    return x <= 0 ? 0 : pow(rate*x, shape) / tgamma(shape) * exp(-rate*x)/x;
}

float gamma_cdf(float x, float shape, float rate)
{
    return x <= 0 ? 0 : ligamma(shape, rate * x) / tgamma(shape);
}

float inverse_gamma(float shape, float scale)
{
    return 1/gamma(shape, scale);
}

float inverse_gamma_pdf(float x, float shape, float scale)
{
    return x <= 0 ? 0 : pow(scale/x, shape) / tgamma(shape) * exp(-scale/x)/x;
}

float inverse_gamma_cdf(float x, float shape, float scale)
{
    return x <= 0 ? 0 : 1 - ligamma(shape, scale / x) / tgamma(shape);
}

void dirichlet(float *dst, float *param, size_t n)
{
    for(int i=0; i < n; i++)
        dst[i] = gamma(param[i], 1);
    normalize(dst, n);
}

float dirichlet_pdf(float *x, float *param, size_t n)
{
    float sum=0, unnorm_pdf=1;
    for(int i=0; i < n; sum += x[i], unnorm_pdf *= pow(x[i], param[i]-1), i++);
    return sum == 1.0 ? unnorm_pdf / beta(x, n) : 0.0;
}
