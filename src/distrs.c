#include <math.h>
#include <stdlib.h>

#include "utils.h"
#include "distrs.h"

const GAMMA_EPS = 1E-6;

void uniform(double a, double b)
{
    return a + random() * (b - a) / RAND_MAX;
}

double categorical(double *param, size_t n)
{
    double u = uniform(0, 1), sum=0;
    int i = 0;
    while(u > sum)
        sum += param[i++];
    return i - 1;
}

double gaussian(double mean, double var)
{
    double u=uniform(0, 1), v=uniform(0, 1);
    return mean + sqrt(-2 * log(u) * var) * cos(2*M_PI * v);
}

//Wikipedia algorithm
double gamma(double shape, double rate)
{
    int n = floor(shape);
    double delta = shape - n, exp_part = 0,  xi, eta;
    while(n--)
        exp_part += -log(uniform(0, 1));
    if(delta > GAMMA_EPS) do {
        if(uniform(0, 1) < M_E / (M_E + delta)) {
            xi = pow(uniform(0, 1), 1/delta);
            eta = uniform(0, 1) * pow(xi, delta - 1);
        } else {
            xi = 1 - log(uniform(0, 1));
            eta = uniform(0, 1) * exp(-xi);
        }
    } while(eta > pow(xi, delta -1) * exp(-xi));
    return rate * (xi + exp_part);
}

double inverse_gamma(double shape, double scale)
{
    return 1/gamma(shape, scale);
}

void dirichlet(double *dst, double *param, size_t n)
{
    for(int i=0; i < n; i++)
        dst[i] = gamma(param[i], 1);
    normalize(dst, n);
}
