#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/lda.h"
#include "../src/utils.h"

const int K = 30;
const int ITERS = 500;
const int TOP_WORDS = 10;

const struct lda_prior PRIOR ={.word_distr_prior=1e-2,
                               .topic_distr_prior=1e-2};

void read_corpus(struct lda_corpus *corp, const char *filename)
{
    unsigned int tot_len=0;
    FILE *corp_file = abort_fopen(filename, "r");
    fscanf(corp_file, "%d %d", &corp->num_docs, &corp->vocab_size);
    corp->doc_lens = (size_t*) abort_calloc(corp->num_docs, sizeof(size_t));
    corp->docs = (unsigned**) abort_calloc(corp->num_docs, sizeof(unsigned*));
    for(int i=0, len; i < corp->num_docs; i++) {
        fscanf(corp_file, "%d", &corp->doc_lens[i]);
        corp->docs[i] = (unsigned*) abort_calloc(corp->doc_lens[i], sizeof(unsigned));
        for(int j=0; i < corp->doc_lens[i]; fscanf(corp_file, "%d", &corp->docs[i][j++]));
    }
    fclose(corp_file);
}

int compare(const void *a, const void *b)
{
    const struct pair_ds *p = (struct pair_ds*) a, *q = (struct pair_ds*) b;
    return p->d < q->d ? -1 : 1;
}

void print_topics(const struct lda_params params, const char *wordsfilename)
{
    FILE *words_file = abort_fopen(wordsfilename, "r");
    size_t vocab_size, max_width;
    fscanf(words_file, "%u %u", &vocab_size, &max_width);
    char words[vocab_size][max_width];
    struct pair_ds topic[vocab_size];
    for(int i=0; i < vocab_size; fscanf(words_file, "%s", words[i++]));
    for(int i=0; i < K; i++) {
        for(int j=0; j < vocab_size; j++) 
            topic[j].d = params.topic_word_distrs[i][j], topic[j].s = words[j];
        qsort(topic, vocab_size, sizeof(struct pair_ds), compare);
        for(int j=0; j < TOP_WORDS; printf("%s ", topic[i++].s));
        puts("");
    }
    fclose(words_file);
}

int main()
{
    struct lda_corpus corpus;
    struct lda_params params;
    struct lda_gibbs_state *gibbs_state;
    srand(time(NULL));

    read_corpus(&corpus, "corpus.txt");
    rand_init_lda_params(&params, corpus, K, PRIOR);

    gibbs_state = alloc_lda_gibbs_state(&params, corpus, K, PRIOR);
    gibbs(gibbs_state, ITERS);
    free_lda_gibbs_state(gibbs_state);

    print_topics(params, "words.txt");

    return 0;
}
