CPP=g++
CFLAGS=--std=c++11 -Wall
MODULE := int_test

all: $(MODULE)

int_test: test/int_test.c src/gmm.h
	$(CPP) $^ $(CFLAGS) -o $@

clean:
	rm -f $(MODULE)
