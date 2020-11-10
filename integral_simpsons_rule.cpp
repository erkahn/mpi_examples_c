#include <stdio.h>
#include "mpi.h"

main(int argc, char** argv){
    int my_rank;
    int p;
    float a = 0.0;
    float b = 1.0;
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
    MPI_Status status;
    
    float Trap(float local_a, float local_b, int local_n, float h);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    h = (b-a)/(n-1);
    local_n = n/p;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    integral = Trap(local_a, local_b, local_n, h);
    
    if(my_rank == 0){
        total = integral;
        for(source = 1; source < p; source++){
            MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
            total = total + integral;
        }
    } else {
        MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);z
    }
    
    if (my_rank == 0){
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %f\n", a, b, total);
    }
    MPI_Finalize();
}

float Trap(
    float local_a,
    float local_b,
    int local_n,
    float h
    ){
    
    float integral;
    float x;
    int i;
    
    float f(float x); /* function we're integrating */
    
    integral = (f(local_a) + f(local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n - 1; i++) {
        x = x + h;
        if(i % 2 == 0)
        integral = integral + 2*f(x);
        else if(i % 2 != 0)
        integral = integral + 4*f(x);
    }
    integral = integral*h/3;
    return integral;
}

float f(float x){
    float return_val;
    return_val = x * x;
    return return_val;
}
