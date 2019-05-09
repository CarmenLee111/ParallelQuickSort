cc = mpicc
CCFLAGS = -O3
LIBS = -lm

BINS = writeArr quicksort

all: $(BINS)

%: %.c
	mpicc $(CCFLAGS) -o $@ $< $(LIBS)

clean:
	$(RM) $(BINS)