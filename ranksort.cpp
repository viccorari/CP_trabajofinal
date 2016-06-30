#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define N 10000

int main(int argc, char *argv[])
{
	double start, stop;
	int x[N], y[N];
	int i, j, my_num, my_place, size, rank, pieces;

	for (i = 0; i<N; i++)
		x[i] =  rand() % n;

	/*********Initialize MPI*******/
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	start = MPI_Wtime();
	MPI_Bcast(&x, N, MPI_INT, 0, MPI_COMM_WORLD);
	pieces = N / size;
	MPI_Bcast(&pieces, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (j = rank*pieces; j<rank*pieces + pieces; j++) {
			my_num = x[j];
			my_place = 0;
			for (i = 0; i<N; i++)
				if (my_num > x[i])
					my_place++;
			if (rank != 0) {
				MPI_Send(&my_place, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
				MPI_Send(&my_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			}
			else {
				y[my_place] = my_num;
				for (i = 1;i<size;i++) {
					MPI_Recv(&my_place, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
					MPI_Recv(&my_num, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
					y[my_place] = my_num;
				}
			}
	}
	stop = MPI_Wtime();
	if (rank == 0) {
	//	for (i = 0; i<N; i++)
	//		printf("%d\n", y[i]);
		printf("%f\n", stop - start);
	}
	MPI_Finalize();
	return 0;
}