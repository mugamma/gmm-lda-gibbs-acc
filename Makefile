CC=gcc
CFLAGS=--std=c99 -Wall -Wpedantic -lm
INIT := init
MODULES := distrs utils gmm gmm_gibbs
TESTS := cont_pdf_test int_test

all: $(INIT) $(MODULES) $(TESTS)

init:
	mkdir -p obj bin

distrs: src/distrs.c src/distrs.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

utils: src/utils.c src/utils.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

gmm: src/gmm.c src/gmm.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

gmm_gibbs: src/gmm_gibbs.c src/gmm.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

cont_pdf_test: test/cont_pdf_test.c
	$(CC) $(CFLAGS) obj/* $^ -o bin/$@ -lm
	./bin/$@

int_test: test/int_test.c
	$(CC) $(CFLAGS) obj/* $^ -o bin/$@ -lm
	./bin/$@

clean:
	rm -rf bin
	rm -rf obj
