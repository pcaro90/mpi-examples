// ---------------------------------------------------
// Copyright (c) 2013 Pablo Caro. All Rights Reserved.
// Pablo Caro <me@pcaro.es> - https://pcaro.es/
// 04-send_recv_02.c
// ---------------------------------------------------

/*
 * This example uses the MPI functions MPI_Send and MPI_Recv to send work from
 * the master to the workers (in this case, a list of numbers), and solutions
 * from the workers to the master (in this example, the sum of the numbers).
 *
 * In this example, the number of rows is independent to the number of
 * processes. The work distribution mechanism used here is slightly better than
 * the one used in the previous example.
 *
 * However, the "COLS" and "ROWS" values are still not dynamic. The next
 * example will improve this.
 */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define COLS 5
#define ROWS 20

int main(int argc, char **argv)
{
    int i, j;

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

    MPI_Status status;

    int slice = ROWS / (nproc - 1);
    int extra_slice = ROWS % (nproc - 1);

    // Master
    if (rank == 0) {
        printf("Master process on processor %s. We have %d processes and %d \
rows to distribute.\n", processor_name, nproc, ROWS);

        // Prepare the matrix and fill it with random data
        srand(time(NULL));
        int *matrix = (int*)malloc(sizeof(int) * ROWS * COLS);

        for (i = 0; i < ROWS; ++i) {
            for (j = 0; j < COLS; ++j) {
                matrix[i * COLS + j] = rand() % 10;
            }
        }

        int count;
        int offset = 0;
        for (i = 0; i < nproc - 1; ++i) {
            count = i + 1 <= extra_slice? slice + 1 : slice;
            MPI_Send(
                &matrix[offset * COLS], // Pointer to the data to send
                count * COLS, // Count of units to send
                MPI_INT, // MPI_Datatype of the units to send
                i + 1, // Destination (worker)
                0, // Tag
                MPI_COMM_WORLD // Communicator
            );
            offset += count;
        }

        int *results = (int*)malloc(sizeof(int) * ROWS);

        offset = 0;
        for (i = 0; i < nproc - 1; ++i) {
            count = i + 1 <= extra_slice? slice + 1 : slice;
            MPI_Recv(
                &results[offset], // Pointer to the data to receive
                count, // Count of units to receive
                MPI_INT, // MPI_Datatype of the units to receive
                i + 1, // Source (worker)
                0, // Tag
                MPI_COMM_WORLD, // Communicator
                &status // Pointer to the status structure
            );
            offset += count;
        }

        for (i = 0; i < ROWS; ++i) {
            for (j = 0; j < COLS; ++j) {
                printf("%2d", matrix[i * COLS + j]);
                printf(" %c ", j != COLS - 1? '+':'=');
            }
            printf("%d\n", results[i]);
        }


    // Workers
    } else {
        int worker_rows = (rank <= extra_slice? slice + 1 : slice);
        int *rows = (int*)malloc(sizeof(int) * worker_rows * COLS);

        MPI_Recv(
                rows, // Pointer to the data to receive
                worker_rows * COLS, // Count of units to receive
                MPI_INT, // MPI_Datatype of the units to receive
                0, // Source (master)
                0, // Tag
                MPI_COMM_WORLD, // Communicator
                &status // Pointer to the status structure
        );

        int *sums = (int*)malloc(sizeof(int) * worker_rows);

        for (i = 0; i < worker_rows; ++i) {
            sums[i] = 0;
            for (j = 0; j < COLS; ++j) {
                sums[i] += rows[i * COLS + j];
            }
        }

        MPI_Send(
            sums, // Pointer to the data to send
            worker_rows, // Count of units to send
            MPI_INT, // MPI_Datatype of the units to send
            0, // Destination (master)
            0, // Tag
            MPI_COMM_WORLD // Communicator
        );

    }

    MPI_Finalize();

    return 0;
}
