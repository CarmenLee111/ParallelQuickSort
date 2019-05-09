#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

int main(int argc, char *argv[]) {
    int n, x;
    if (argc != 3){
        printf("Usage: ./random_generator <num> <max>\n");
        return -1;
    }

    n = atoi(argv[1]);
    x = atoi(argv[2]);

    FILE *fp;
    time_t t;

    srand( (unsigned) time(&t));
    fp = fopen("input", "w");
    printf("Generate %d random numbers with max value %d\n", n, x);
    int i, d;
    fwrite(&n, sizeof(int), 1, fp);
    for (i=0; i<n; i++) {
        d = (int) (rand()%x);
        printf("%d ", d);
        fwrite(&d, sizeof(int), 1, fp);
    }
    printf("\n");
    fclose(fp);
    return 0;
}