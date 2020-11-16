#define N 4
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


void print_results(char *prompt, int a[N][N]);
void Get_data2();

int main(int argc, char *argv[]){

    int i;
    int j;
    int k;
    int rank;
    int size;
    int sum = 0;
    int a[N][N]={{1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7}};
    int b[N][N]={{1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7}};
    int c[N][N];
    int aa[N],cc[N];
    int columnIndex, rowIndex;
    
    MPI_Init(&argc, &argv);
    if(rank == 0)
        Get_data2();
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    //scatter rows of first matrix to different processes
    MPI_Scatter(a, N*N/size, MPI_INT, aa, N*N/size, MPI_INT,0,MPI_COMM_WORLD);
    
    //broadcast second matrix to all processes
    MPI_Bcast(b, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    //perform vector multiplication by all processes
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            sum = sum + aa[j] * b[j][i];
        }
        cc[i] = sum;
        sum = 0;
    }
    
    MPI_Gather(cc, N*N/size, MPI_INT, c, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        print_results("C = ", c);
    MPI_Finalize();
}

void print_results(char *prompt, int a[N][N]){
    int i, j;
    printf ("\n\n%s\n", prompt);
    for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                    printf(" %d", a[i][j]);
            }
            printf ("\n");
    }
    printf ("\n\n");
}

void Get_data2(
    int row,
    int column,
    int matrix1[N][N],
    int matrix2[N][N],
    int my_rank){
}
