#pragma once

#include <math.h>

// add `u` and `v` of length `k` and store the result in `dst`
void vec_add(double *dst, double *u, double *v, size_t k);

void normalize(double *v, size_t n);

inline double square(double x) { return x * x; }

double ligamma(double s, double x);

double uigamma(double s, double x) { return tgamma(s) - ligamma(s, x); }

double beta(double *x, size_t n);
