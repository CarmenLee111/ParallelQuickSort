#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char const *argv[])
{ 
    srand(time(NULL)); 
    int N = 500;
    FILE *fp;
    fp = fopen("arr","w");
    if (!fp){                           
      printf("Error: Could not open file for Writing\n");  
      return -1;
    } 
    fwrite(&N,sizeof(int),1,fp);
    for (int i = 0; i < N; i++){
        //int temp = i%16;
        int temp = rand()%100;
        printf("%d\n",temp);
        fwrite(&temp,sizeof(int),1,fp);
    }
    fclose(fp);
    return 0;
}