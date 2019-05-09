# Make test file

Generate a file named 'input.txt' that contains 100 elements in the range of (0,20)
```bash
$ gcc -o random_generator random_generator.c
$ ./random_generator 100 20
```


# Make executable
```bash
$ Make
```


# Execute on 4 threads and strategy 3
```bash
$ mpirun -n 4 input.txt output.txt 3
```