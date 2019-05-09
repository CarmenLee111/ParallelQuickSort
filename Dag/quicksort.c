#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define MASTER 0 
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int arrSize;
int depth = 1;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void globalsort(int pivotStrat, int rank, int groupSize, int *subArray, int subArrayLength, MPI_Comm commLocal){
    int i = 0;
    double pivot;
    int interval;
    int *arrayTotal;
    MPI_Status status;

    qsort(subArray, subArrayLength, sizeof(int), cmpfunc); //local sort
    if (groupSize == 1){
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &groupSize);
        int *sortedArray = (int*)malloc(arrSize * sizeof(int));
        int *recCount = (int*)calloc(groupSize, sizeof(int));
        int *disp = (int*)calloc(groupSize, sizeof(int));
        MPI_Gather(&subArrayLength,1,MPI_INT,recCount,1,MPI_INT,0,MPI_COMM_WORLD);
        for (int j = 0; j < groupSize - 1; j++){
            disp[j+1] += recCount[j];
            disp[j+1] += disp[j];
        }
        MPI_Gatherv(subArray,subArrayLength,MPI_INT,sortedArray,recCount,disp,MPI_INT,0,MPI_COMM_WORLD);
        if (rank == 0){
            for(int j = 0; j < arrSize; j++){
                printf("%d\n",sortedArray[j]);
            }
            printf("%d numbers\n",arrSize);
        }
        return;
    }
    if (pivotStrat == 1){
        if (subArrayLength%2 == 0){
            if (subArrayLength == 0){
                pivot = subArray[subArrayLength/2]; 
            } else {
                pivot = 0;
            }
        } else {
            pivot = (double)(subArray[subArrayLength/2] + subArray[1 + subArrayLength/2])/2;
        }
        MPI_Bcast(&pivot, 1, MPI_DOUBLE, groupSize - 1, commLocal); //brodcast pivot in subgroup
    }
    while(i < subArrayLength && subArray[i] < pivot){
        i++;
    }
    int *arrayLarger = &subArray[i]; //split data
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(commLocal);
    if (rank/((groupSize/2)) == 0) {
        MPI_Send(arrayLarger, subArrayLength - i, MPI_INT, (groupSize/2) + rank, rank, commLocal); //sending part

        int incommingSize; //recieving part
        MPI_Probe((groupSize/2) + rank, (groupSize/2) + rank, commLocal, &status);
        MPI_Get_count(&status, MPI_INT, &incommingSize);
        int *arrayLesser = (int*)malloc(incommingSize * sizeof(int));
        MPI_Recv(arrayLesser, incommingSize, MPI_INT, (groupSize/2) + rank, (groupSize/2) + rank, commLocal, &status);
        
        interval = incommingSize + i;
        arrayTotal = (int*)malloc(interval * sizeof(int));
        memcpy(arrayTotal, subArray, i * sizeof(int)); 
        memcpy(arrayTotal + i, arrayLesser, incommingSize * sizeof(int));
    } else {
        int incommingSize; //recieving part
        MPI_Probe(rank%(groupSize/2), rank%(groupSize/2), commLocal, &status);
        MPI_Get_count(&status, MPI_INT, &incommingSize);
        int *arrayLargerInc = (int*)malloc(incommingSize * sizeof(int));
        MPI_Recv(arrayLargerInc, incommingSize, MPI_INT, rank%(groupSize/2), rank%(groupSize/2), commLocal, &status);

        MPI_Send(subArray, i, MPI_INT, rank%(groupSize/2), rank, commLocal); //sending part

        interval = incommingSize + (subArrayLength - i);
        arrayTotal = (int*)malloc(interval * sizeof(int));
        memcpy(arrayTotal, arrayLarger, (subArrayLength - i) * sizeof(int)); 
        memcpy(arrayTotal + (subArrayLength - i), arrayLargerInc, incommingSize * sizeof(int));
    }
    MPI_Barrier(commLocal);
    int glRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &glRank);
    int color = rank/(groupSize/2); //create subgroup communication
    MPI_Comm_split(commLocal, color, rank, &commLocal);
    
    MPI_Comm_size(commLocal, &groupSize); 
    MPI_Comm_rank(commLocal, &rank); 
    
    depth += 1;
    //call next level globalsort
    globalsort(pivotStrat, rank, groupSize, arrayTotal, interval, commLocal);
    return;
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv); // Initialize MPI 
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the number of processors 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get my number  

    if (argc!=4){
        printf("Wrong number of input arguments\n");
        return -1;
    }
    char *filenameInput = strdup(argv[1]);
    char *filenameOutput = strdup(argv[2]);
    const int pivotStrat = atoi(argv[3]);
    
    FILE *fp;
    fp = fopen(filenameInput, "r");
    if (!fp){                           
      printf("Error: Could not open file for reading\n");  
      return -1;
    } 
    size_t trash;
    trash = fread(&arrSize, sizeof(int), 1, fp);
    int *array = (int*)malloc(arrSize * sizeof(int));

    for(int i = 0; i < arrSize; i++){
        trash = fread(&(array[i]),sizeof(int),1,fp);
        //printf("%d\n",array[i]);
    }

    int initialInterval = arrSize/size; //TODO account for residual
    int residual = arrSize%size;
    int initialIntervalRes = initialInterval;
    if (rank < residual){
        initialIntervalRes += 1;
    }

    int *subArray = (int*)malloc(initialIntervalRes * sizeof(int)); //divide into local datasets
    //printf("Init %d, length %d, rank%d\n",rank*initialInterval + MIN(residual, rank),initialIntervalRes, rank);
    memcpy(subArray, &array[rank*initialInterval + MIN(residual, rank)], initialIntervalRes*sizeof(*array));
    int color = 0; //create subgroup communication
    MPI_Comm commLocal;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &commLocal);

    
    globalsort(pivotStrat, rank, size, subArray, initialIntervalRes, commLocal); //call globalsort
    

    MPI_Finalize(); /* Shut down and clean up MPI */
}