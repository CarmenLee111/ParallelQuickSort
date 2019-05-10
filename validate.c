#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

int prev = 0;
int curr = 0;


int load_input(int** l, char *input_file, char *output_file) {


  FILE *fp = fopen(input_file, "r");
  int n;
  if (!fp) {
    printf("load_data error: failed to open input file '%s'.\n", input_file);
    return -1;
  }
  fscanf(fp, "%d", &n);
  printf("n: %d\n", n);
  *l = malloc(sizeof(int) * n);
  int i;
  fclose(fp);

  FILE *f = fopen(output_file, "r");  
  for (i=0; i<n; i++) {
      fscanf(f, "%d", &((*l)[i]));
  }
  fclose(f);
  
  return n;
}

int main(int argc, char *argv[]) {

    int n;
    int* arr;


    char *inputfile = argv[1];
    char *outputfile = argv[2];

    n = load_input(&arr, inputfile, outputfile);

  for(int i = 0; i<n; i++){
      //printf("%d ", arr[i]);

      curr = arr[i];
      if(curr < prev){
          printf("\nElement of index %d, is smaller than previous since, %d < %d\n", i, curr, prev );
      }
      prev = curr;
  }
}


