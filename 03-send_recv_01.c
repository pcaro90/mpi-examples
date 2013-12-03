// ---------------------------------------------------
// Copyright (c) 2013 Pablo Caro. All Rights Reserved.
// Pablo Caro <me@pcaro.es> - https://pcaro.es/
// 03-send_recv_01.c
// ---------------------------------------------------

/*
 * This example uses the MPI functions MPI_Send and MPI_Recv to send work from
 * the master to the workers (in this case, a list of numbers), and solutions
 * from the workers to the master (in this example, the sum of the numbers).
 *
 * This example uses as many rows as processes. This is the easiest way to
 * distribute the work. The next examples will feature a different way to work,
 * making the number of elements independent to the number of workers.
 */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

// The number of rows is the same than the number of workers
#define COLS 5

int main(int argc, char **argv)
{
    int i, j;
    int **matrix;
    int *row;
    int *results;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Establishes the size of the group
    int nproc;
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    const int ROWS = nproc - 1;

    // Establishes the rank of the calling process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Gets the name of the processor
    int processor_name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(processor_name, &processor_name_len);

    MPI_Status status;

    // Master
    if (rank == 0) {
        printf("Master process on processor %s. We have %d processes.\n", processor_name, nproc);

        // Prepare the matrix and fill it with random data
        srand(time(NULL));
        matrix = (int**)malloc(sizeof(int) * ROWS * COLS);

        for (i = 0; i < ROWS; ++i) {
            matrix[i] = (int*)malloc(sizeof(int) * COLS);
            for (j = 0; j < COLS; ++j) {
                matrix[i][j] = rand() % 10;
            }
        }

        for (i = 0; i < ROWS; ++i) {
            MPI_Send(
                &matrix[i][0], // Pointer to the data to send
                COLS, // Count of units to send
                MPI_INT, // MPI_Datatype of the units to send
                i + 1, // Destination (worker)
                0, // Tag
                MPI_COMM_WORLD // Communicator
            );
        }

        results = (int*)malloc(sizeof(int) * ROWS);

        for (i = 0; i < ROWS; ++i) {
            MPI_Recv(
                &results[i], // Pointer to the data to receive
                1, // Count of units to receive
                MPI_INT, // MPI_Datatype of the units to receive
                i + 1, // Source (worker)
                0, // Tag
                MPI_COMM_WORLD, // Communicator
                &status // Pointer to the status structure
            );
        }

        for (i = 0; i < ROWS; ++i) {
            for (j = 0; j < COLS; ++j) {
                printf("%2d", matrix[i][j]);
                printf(" %c ", j != COLS - 1? '+':'=');
            }
            printf("%d\n", results[i]);
        }


    // Workers
    } else {
        row = (int*)malloc(sizeof(int) * COLS);
        MPI_Recv(
                row, // Pointer to the data to receive
                COLS, // Count of units to receive
                MPI_INT, // MPI_Datatype of the units to receive
                0, // Source (master)
                0, // Tag
                MPI_COMM_WORLD, // Communicator
                &status // Pointer to the status structure
        );

        int sum = 0;
        for (i = 0; i < COLS; ++i) {
            sum += row[i];
        }

        MPI_Send(
            &sum, // Pointer to the data to send
            1, // Count of units to send
            MPI_INT, // MPI_Datatype of the units to send
            0, // Destination (master)
            0, // Tag
            MPI_COMM_WORLD // Communicator
        );

    }

    MPI_Finalize();

    return 0;
}
