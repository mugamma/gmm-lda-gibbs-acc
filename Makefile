CC=gcc
CFLAGS=--std=c99 -Wall -Wpedantic -lm
INIT := init
BASE_MODULES := distrs utils 
BASE_TESTS := cont_pdf_test cont_gof_test
GMM_MODULES := gmm gmm_gibbs 
GMM_TESTS := gmm_int_test 
LDA_MODULES := lda lda_gibbs 
LDA_TESTS := lda_int_test 
BASE = $(BASE_MODULES) $(BASE_TESTS)
GMM = $(GMM_MODULES) $(GMM_TESTS)
LDA = $(LDA_MODULES) $(LDA_TESTS)

all: $(INIT) $(BASE) $(GMM) $(LDA)

init:
	mkdir -p obj bin

distrs: src/distrs.c src/distrs.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

utils: src/utils.c src/utils.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

cont_pdf_test: test/cont_pdf_test.c
	$(CC) $(CFLAGS) obj/* $^ -o bin/$@ -lm
	./bin/$@

cont_gof_test: test/cont_gof_test.c
	$(CC) $(CFLAGS) obj/* $^ -o bin/$@ -lm
	./bin/$@

gmm: src/gmm.c src/gmm.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

gmm_gibbs: src/gmm_gibbs.c src/gmm.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

gmm_int_test: test/gmm_int_test.c
	$(CC) $(CFLAGS) obj/* $^ -o bin/$@ -lm
	./bin/$@

lda: src/lda.c src/lda.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

lda_gibbs: src/lda_gibbs.c src/lda.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

lda_int_test: test/lda_int_test.c
	$(CC) $(CFLAGS) obj/* $^ -o bin/$@ -lm
	./bin/$@

clean:
	rm -rf bin
	rm -rf obj
