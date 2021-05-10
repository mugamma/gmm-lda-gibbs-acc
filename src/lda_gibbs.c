#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lda.h"
#include "utils.h"
#include "distrs.h"

struct lda_sufficient_statistic {
    // `ns[i][k] = n` means n words in the i-th doc come form the k-th topic
    size_t **ns; 

    // `ms[k][j] = m` the j-th word appears m times associated with the k-th
    // topic
    size_t **ms; 
};

struct lda_gibbs_state {
    // number of data points
    size_t num_topics;

    struct lda_prior prior;
    struct lda_corpus corpus;
    struct lda_params *params;
    struct lda_sufficient_statistic *ss;
};

void alloc_lda_ss(struct lda_gibbs_state *s)
{
    size_t num_docs=s->corpus.num_docs, num_topics=s->num_topics,
           vocab_size=s->corpus.vocab_size;
    s->ss = (struct lda_sufficient_statistic*)
        abort_malloc(sizeof(struct lda_sufficient_statistic));
    s->ss->ns = (size_t **) abort_calloc(num_docs, sizeof(size_t *));
    s->ss->ms = (size_t **) abort_calloc(num_topics, sizeof(size_t *));
    for(int i=0; i < num_docs; i++)
        s->ss->ns[i] = (size_t *) abort_calloc(num_topics, sizeof(size_t));
    for(int k=0; k < num_topics; k++)
        s->ss->ms[k] = (size_t *) abort_calloc(vocab_size, sizeof(size_t));
}

void free_lda_ss(struct lda_gibbs_state *s)
{
    for(int i=0; i < s->corpus.num_docs; free(s->ss->ns[i++]));
    for(int k=0; k < s->num_topics; free(s->ss->ms[k++]));
    free(s->ss->ns);
    free(s->ss->ms);
    free(s->ss);
}

struct lda_gibbs_state
*alloc_lda_gibbs_state(struct lda_params *params, struct lda_corpus corpus,
                       size_t num_topics, struct lda_prior prior)
{
    struct lda_gibbs_state *s = abort_malloc(sizeof(struct lda_gibbs_state));
    s->num_topics = num_topics;
    s->prior = prior;
    s->corpus = corpus;
    s->params = params;
    alloc_lda_ss(s);
    return s;
}

void free_lda_gibbs_state(struct lda_gibbs_state *state)
{
    free_lda_ss(state);
    free(state);
}

void clear_sufficient_statistic(struct lda_gibbs_state *state)
{
    size_t num_topics=state->num_topics, vocab_size=state->corpus.vocab_size;
    for(int i=0; i < state->corpus.num_docs; i++)
        memset(state->ss->ns[i], 0, num_topics * sizeof(size_t));
    for(int k=0; k < state->num_topics; k++)
        memset(state->ss->ms[k], 0, vocab_size * sizeof(size_t));
}

void update_sufficient_statistic(struct lda_gibbs_state *state)
{
    // XXX XXX this is the function that needs to be accelerated.
    clear_sufficient_statistic(state);
    for(size_t i=0; i < state->corpus.num_docs; i++) {
        for(size_t j=0; j < state->corpus.doc_lens[i]; j++) {
            unsigned int z=state->params->zs[i][j], w=state->corpus.docs[i][j];
            state->ss->ns[i][z]++;
            state->ss->ms[z][w]++;
        }
    }
}

void update_topics(struct lda_gibbs_state *state)
{
    size_t vocab_size=state->corpus.vocab_size, **ms=state->ss->ms;
    double dir_param[vocab_size], eta=state->prior.word_distr_prior;
    for(int k=0; k < state->num_topics; k++) {
        for(int j=0; j < vocab_size; j++)
            dir_param[j] = eta + ms[k][j];
        dirichlet(state->params->topic_word_distrs[k], dir_param, vocab_size);
    }
}

void update_topic_distrs(struct lda_gibbs_state *state)
{
    size_t num_topics=state->num_topics, **ns=state->ss->ms;
    double dir_param[num_topics], alpha=state->prior.topic_distr_prior;
    for(int i=0; i < state->corpus.num_docs; i++) {
        for(int k=0; k < num_topics; k++)
            dir_param[k] = alpha + ns[i][k];
        dirichlet(state->params->doc_topic_distrs[i], dir_param, num_topics);
    }
}

void update_zs(struct lda_gibbs_state *state)
{
    double weights[state->num_topics], **theta=state->params->doc_topic_distrs,
           **beta=state->params->topic_word_distrs;
    for(int i=0; i < state->corpus.num_docs; i++) {
        for(int j=0; j < state->corpus.doc_lens[i]; j++) {
            unsigned int w = state->corpus.docs[i][j];
            for(int k=0; k < state->num_topics; k++)
                weights[k] = theta[i][k] * beta[k][w];
            normalize(weights, state->num_topics);
            state->params->zs[i][j] = categorical(weights, state->num_topics);
        }
    }
}

void gibbs(struct lda_gibbs_state *state, size_t iters)
{
    while(iters--) {
        update_sufficient_statistic(state);
        update_topics(state);
        update_topic_distrs(state);
        update_zs(state);
    }
}
