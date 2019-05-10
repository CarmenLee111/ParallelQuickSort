cc = mpicc
CCFLAGS =
LIBS = -lm

BINS = quicksort validate

all: $(BINS)

quicksort:main.c
	mpicc -o quicksort main.c $(LIBS)

clean:
	rm $(BINS)