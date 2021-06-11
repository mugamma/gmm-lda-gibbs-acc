#pragma once

float uniform(float a, float b);
float uniform_pdf(float x, float a, float b);
float uniform_cdf(float x, float a, float b);

int categorical(float *param, size_t n);
float categorical_pdf(int x, float *param, size_t n);
float categorical_cdf(int x, float *param, size_t n);

float gaussian(float mean, float var);
float gaussian_pdf(float x, float mean, float var);
float gaussian_cdf(float x, float mean, float var);

float gamma(float shape, float rate);
float gamma_pdf(float x, float shape, float rate);
float gamma_cdf(float x, float shape, float rate);

float inverse_gamma(float shape, float scale);
float inverse_gamma_pdf(float x, float shape, float scale);
float inverse_gamma_cdf(float x, float shape, float scale);

// sample from an `n`-dimensional dirichlet in `dst`
void dirichlet(float *dst, float *param, size_t n);
float dirichlet_pdf(float *x, float *param, size_t n);
