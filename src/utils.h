#pragma once

#include <math.h>

// add `u` and `v` of length `k` and store the result in `dst`
void vec_add_dd(float *dst, float *u, float *v, size_t k);
void vec_add_ud(float *dst, unsigned int *u, float *v, size_t k);

void normalize(float *v, size_t n);

float square(float x);

float ligamma(float s, float x);

extern float uigamma(float s, float x);

float beta(float *x, size_t n);

// try to malloc/calloc and abort if unsuccessful
void *abort_calloc(size_t nmemb, size_t size);
void *abort_malloc(size_t size);
