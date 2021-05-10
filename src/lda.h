#pragma once

#include <stddef.h>

struct lda_corpus {
    size_t num_docs;
    size_t *doc_lens;
    size_t vocab_size;

    unsigned int **docs; // decidedly not tiled
};

struct lda_prior {
    // symmetric Dirichlet prior on word distribution of each topic
    double word_distr_prior;

    // symmetric Dirichlet prior on topic distribution of each document
    double topic_distr_prior;
};

struct lda_params {
    double **doc_topic_distrs; // decidedly not tiled
    double **topic_word_distrs; // decidedly not tiled

    unsigned int **zs; // decidedly not tiled
};

struct lda_gibbs_state;

void rand_init_lda_params(struct lda_params *params,
                          struct lda_corpus corpus,
                          unsigned int num_topics,
                          struct lda_prior prior);

struct lda_gibbs_state *alloc_lda_gibbs_state(struct lda_params *params,
                                              struct lda_corpus corpus,
                                              size_t num_topics,
                                              struct lda_prior prior);

void free_lda_gibbs_state(struct lda_gibbs_state *state);

void gibbs(struct lda_gibbs_state *state, size_t iters);
