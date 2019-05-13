cc = mpicc 
CCFLAGS = -O3 -funroll-loops -march=native -ffast-math
LIBS = -lm

BINS = quicksort validate random_generator

all: $(BINS)

random_generator:random_generator.c
	mpicc $(CFLAGS) -o random_generator random_generator.c $(LIBS)
	
quicksort:main.c
	mpicc $(CFLAGS) -o quicksort main.c $(LIBS)

clean:
	rm $(BINS)