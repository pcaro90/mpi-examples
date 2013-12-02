#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{

  // Initialize MPI
  MPI_Init(&argc, &argv);

  // Establishes the size of the group
  int nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  // Establishes the rank of the calling process
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Gets the name of the processor
  int processor_name_len;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(processor_name, &processor_name_len);

  printf("Process %d on %s out of %d\n", rank, processor_name, nproc);

  MPI_Finalize();

  return 0;
}
