#pragma once

#include <math.h>
#include <stdio.h>

// helper data type for pair of double and string
struct pair_ds {
    double d;
    char *s;
};

// add `u` and `v` of length `k` and store the result in `dst`
void vec_add_dd(double *dst, double *u, double *v, size_t k);
void vec_add_ud(double *dst, unsigned int *u, double *v, size_t k);

void normalize(double *v, size_t n);

double square(double x);

double ligamma(double s, double x);

extern double uigamma(double s, double x);

double beta(double *x, size_t n);

// try to malloc/calloc and abort if unsuccessful
void *abort_calloc(size_t nmemb, size_t size);
void *abort_malloc(size_t size);

FILE *abort_fopen(const char *pathname, const char *mode);
