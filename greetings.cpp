#include <stdio.h>
#include <string.h>
#include "mpi.h"

main(int argc, char* argv[]){
int my_rank;		/*rank of process*/
int p;			/*number of process*/
int source;		/*rank of sender*/
int dest;		/*rank of receiver*/
int tag = 0;		/*tag for messages*/
char message[100];	/*store for messages*/
MPI_Status status;	/*receive*/

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
MPI_Comm_size(MPI_COMM_WORLD, &p);

/*int MPI_Send(
    void* message,
    int count,
    MPI_Datatype datatype,
    int dest,
    int tag,
    MPI_Comm comm);

int MPI_Recv(
    void* message,
    int count,
    MPI_Datatype datatype,
    int source
    int tag,
    MPI_Comm comm,
    MPI_Status* status)*/

if (my_rank != 0){
 sprintf(message, "Greetings from process %d", my_rank);
 dest = 0;
 MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
}
else{ /*when rank is 0*/
 for(source = 1; source < p; source++){
  MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
  printf("%s\n", message);
 }
}

MPI_Finalize();

}
