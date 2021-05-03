#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void vec_add(double *dst, double *u, double *v, size_t k)
{
    for(int i=0; i < k; i++)
        dst[i] = u[i] + v[i];
}

void normalize(double *v, size_t n)
{
    double sum = 0;
    for(int i=0; i < n; i++)
        sum += v[i];
    for(int i=0; i < n; i++)
        v[i] /= sum;
}

double ligamma(double s, double x)
{
    double sum=0, term=1/s;
    for(int k=1; term < DBL_EPSILON; k++) {
        sum += term;
        term *= -x/k * (s + k - 1)/(s + k);
    }
    return pow(x, s) * sum;
}

double beta(double *x, size_t n)
{
    double gamma_prod=1, sum=0;
    for(int i=0; i < n; gamma_prod *= tgamma(x[i]), sum += x[i], i++);
    return gamma_prod / tgamma(sum);
}

void *abort_calloc(size_t nmemb, size_t size)
{
    void *mem = calloc(nmemb, size);
    if(mem == NULL)
        fputs("insufficient memory", stderr), abort();
    return mem;
}
