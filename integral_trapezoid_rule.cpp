#include <stdio.h>
#include "mpi.h"

float f(float x, float fN1); /* function we're integrating */
float Trap(float local_a, float local_b, int local_n, float h,float fN1);
int chooseFunction();

main(int argc, char** argv){
    int my_rank;
    int p;
    float a = 0;
    float b = 1;
    int n = 1024;
    float h;
    float local_a;
    float local_b;
    int local_n;
    float integral;
    float total;
    int source;
    int dest = 0;
    int tag = 0;
    int fN1 = 1;

    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    h = (b-a)/n;
    local_n = n/p;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    integral = Trap(local_a, local_b, local_n, h, fN1);
    
    if(my_rank == 0){
        total = integral;
        for(source = 1; source < p; source++){
            MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
            total = total + integral;
        }
    } else {
        MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
    }
    
    if (my_rank == 0){
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f - %f\n", a, b, total);
    }
    MPI_Finalize();
}

float Trap(
    float local_a,
    float local_b,
    int local_n,
    float h,
    float fN1
    ){
    
    float integral;
    float x;
    int i;
    
    integral = (f(local_a, fN1) + f(local_b, fN1))/2.0;
    x = local_a;
    for (i = 1; i <= local_n - 1; i++) {
        x = x + h;
        integral = integral + f(x, fN1);
    }
    integral = integral*h;
    return integral;
}

float f(float x, float fN1){
    float return_val;
    if(fN1 == 1)
        return_val = x * x;
    else if (fN1 == 2)
        return_val = 2 * x + 1;
    else {
        printf("Please choose correct number\n");
        return return_val;
    }
}
