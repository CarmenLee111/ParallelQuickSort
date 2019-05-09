cc = mpicc
CCFLAGS =
LIBS = -lm

BINS = quicksort

all: $(BINS)

quicksort:main.c
	mpicc -o quicksort main.c $(LIBS)

clean:
	rm $(BINS)