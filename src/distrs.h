#pragma once

double uniform(double a, double b);

double categorical(double *param, size_t n);

double gaussian(double mean, double var);

double gamma(double shape, double rate);

double inverse_gamma(double shape, double rate);

// sample from an `n`-dimensional dirichlet in `dst`
void dirichlet(double *dst, double *param, size_t n);
