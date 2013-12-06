// ---------------------------------------------------
// Copyright (c) 2013 Pablo Caro. All Rights Reserved.
// Pablo Caro <me@pcaro.es> - https://pcaro.es/
// 05-send_recv_03.c
// ---------------------------------------------------

/*
 * This example uses the MPI functions MPI_Send and MPI_Recv to send work from
 * the master to the workers. In this case, the product of two matrices will be
 * calculated.
 *
 * In this example, the size of the matrices is independent to the number of
 * processes. Also, the workers don't know this size until the master gives
 * them the information.
 *
 * In order to use the data smartly, the A matrix will be stored as a list of
 * its rows, while the B matrix will be stored as a list of its columns.
 *
 * In this example, the B matrix will be sent to every worker, even if they
 * don't need every column. This could be improved, and it would be useful in
 * the case of very big matrices.
 */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

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

    // Master
    if (rank == 0) {
        const int AR = 10;
        const int AC = 10;

        const int BR = AC;
        const int BC = 10;

        const int CR = AR;
        const int CC = BC;

        printf("Master process on processor %s. We have %d processes.\n", processor_name, nproc, ROWS);
        printf("Matrix A: %dx%d    Matrix B: %dx%d\n", AR, AR, BR, BR);

        // Prepare the matrices and fill them with random data
        srand(time(NULL));

        int *A = (int*)malloc(sizeof(int) * AR * AC);
        for (i = 0; i < AR; ++i) {
            for (j = 0; j < AC; ++j) {
                A[i * AC + j] = rand() % 10;
            }
        }

        int *B = (int*)malloc(sizeof(int) * BR * BC);
        for (i = 0; i < BC; ++i) {
            for (j = 0; j < BR; ++j) {
                /* B[i * BC + j] = rand() % 10; */
                b[i * BR + j] = i == j? 1 : 0;
            }
        }

        int total_jobs = CR * CC;

        int slice = total_jobs / (nproc - 1);
        int extra_slice = total_jobs % (nproc - 1);

        int count;
        int offset = 0;
        int info[2] = {count, AC};

        for (i = 0; i < nproc - 1; ++i) {
            count = i + 1 <= extra_slice? slice + 1 : slice;
            MPI_Send(info, 2, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&A[offset * AC], count * AC, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
            MPI_Send(&B[offset * BR], count * BR, MPI_INT, i + 1, 2, MPI_COMM_WORLD);
            offset += count;
        }

        int *C = (int*)malloc(sizeof(int) * CR * CC);

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

        int info[2];
        MPI_Recv(info, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);

        int count = info[0];
        int *rows = (int*)malloc(sizeof(int) * info[1] * count);
        int *cols = (int*)malloc(sizeof(int) * info[1] * count);

        MPI_Recv(rows, info[1] * count, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(cols, info[1] * count, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

        int *sums = (int*)malloc(sizeof(int) * worker_rows);
        for (i = 0; i < count; ++i) {
            for (j = 0; j < info[1]; ++j) {

            }
        }


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
