#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void vec_add_dd(float *dst, float *u, float *v, size_t k)
{
    for(int i=0; i < k; i++)
        dst[i] = u[i] + v[i];
}

void vec_add_ud(float *dst, unsigned int *u, float *v, size_t k)
{
    for(int i=0; i < k; i++)
        dst[i] = u[i] + v[i];
}

void normalize(float *v, size_t n)
{
    float sum = 0;
    for(int i=0; i < n; i++)
        sum += v[i];
    for(int i=0; i < n; i++)
        v[i] /= sum;
}

float square(float x) { return x*x; }

float ligamma(float s, float x)
{
    float sum=0, term=1/s;
    for(int k=1; term >= DBL_EPSILON; sum += term, term *= x/(s + k), k++);
    return pow(x, s) * exp(-x) * sum; 
}

float uigamma(float s, float x) { return tgamma(s) - ligamma(s, x); }

float beta(float *x, size_t n)
{
    float gamma_prod=1, sum=0;
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

void *abort_malloc(size_t size) { return abort_calloc(1, size); }
