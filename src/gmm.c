#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "distrs.h"
#include "gmm.h"

void alloc_dirichlet_param(double **dirichlet_param, double param, size_t k)
{
    *dirichlet_param = (double *) calloc(k, sizeof(double));
    if(dirichlet_param == NULL) {
        fputs("insufficient memory.", stderr);
        exit(1);
    }
    while(k--)
        *dirichlet_param[k] = param;
}

void free_dirichlet_param(double *dirichlet_param)
{
    free(dirichlet_param);
}

void rand_init_gmm_params(struct gmm_params *params, size_t n, size_t k,
                          struct gmm_prior prior)
{
    double *dirichlet_param;
    alloc_dirichlet_param(&dirichlet_param, prior.dirichlet_prior, k);
    dirichlet(params->weights, dirichlet_param, k);
    for(int j=0; j < k; j++) {
        params->means[j] = gaussian(prior.means_mean_prior,
                                    prior.means_var_prior);
        params->vars[j] = inverse_gamma(prior.vars_shape_prior,
                                        prior.vars_rate_prior);
    }
    for(int i=0; i < n; params->zs[i++] = categorical(params->weights, k));
    free_dirichlet_param(dirichlet_param);
}
