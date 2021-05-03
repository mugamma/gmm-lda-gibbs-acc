CC=gcc
CFLAGS=--std=c99 -Wall -Wpedantic -lm
TESTS := int_test
MODULES := distrs utils gmm gmm_gibbs

all: $(MODULES) $(TESTS)

distrs: src/distrs.c src/distrs.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

utils: src/utils.c src/utils.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

gmm: src/gmm.c src/gmm.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

gmm_gibbs: src/gmm_gibbs.c src/gmm.h
	$(CC) -c $(CFLAGS) $< -o obj/$@.o

int_test: test/int_test.c
	$(CC) $(CFLAGS) obj/* $^ -o bin/$@
	./bin/$@

clean:
	rm -f bin/*
	rm -f obj/*
