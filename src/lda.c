#include "lda.h"
#include "distrs.h"

void rand_init_lda_params(struct lda_params *params,
                          struct lda_corpus corpus,
                          unsigned int num_topics,
                          struct lda_prior prior)
{
    double topic_param[num_topics], word_param[corpus.vocab_size];
    for(int k=0; k < num_topics; topic_param[k++] = prior.topic_distr_prior);
    for(int j=0; j < corpus.vocab_size; word_param[j++] = prior.word_distr_prior);
    for(int k=0; k < num_topics; k++)
        dirichlet(params->doc_topic_distrs[k], topic_param, num_topics);
    for(int j=0; j < corpus.vocab_size; j++)
        dirichlet(params->topic_word_distrs[j], word_param, corpus.vocab_size);
    for(int i=0; i < corpus.num_docs; i++)
        for(int j=0; j < corpus.doc_lens[i]; j++)
            params->zs[i][j++] = categorical(params->doc_topic_distrs[i],
                                                         num_topics);
}

