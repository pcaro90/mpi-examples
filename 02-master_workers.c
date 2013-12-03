// ---------------------------------------------------
// Copyright (c) 2013 Pablo Caro. All Rights Reserved.
// Pablo Caro <me@pcaro.es> - https://pcaro.es/
// 02-master_slave.c
// ---------------------------------------------------

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

    if (rank == 0) {
        printf("Master process on processor %s. We have %d processes.\n", processor_name, nproc);
    } else {
        printf("Worker %2d on processor %s\n", rank, processor_name);
    }

    MPI_Finalize();

    return 0;
}
