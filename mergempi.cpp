#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int *, int *, int, int, int);
void mergeSort(int *, int *, int, int);

int main(int argc, char** argv) {
	double start, stop;
	int n = 10000;
	int *original_array = malloc(n * sizeof(int));
	
	int c;
	srand(time(NULL));
	for (c = 0; c < n; c++) {

		original_array[c] = rand() % n;
		//printf("%d ", original_array[c]);

	}


	/********** Initialize MPI **********/
	int world_rank;
	int world_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	start = MPI_Wtime();

	int size = n / world_size;

	int *sub_array = malloc(size * sizeof(int));
	MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

	int *tmp_array = malloc(size * sizeof(int));
	mergeSort(sub_array, tmp_array, 0, (size - 1));

	int *sorted = NULL;
	if (world_rank == 0) {

		sorted = malloc(n * sizeof(int));
		
	}

	MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

	if (world_rank == 0) {

		int *other_array = new int[n]; 
		mergeSort(sorted, other_array, 0, (n - 1));

		/*printf("this is the sorted array: ");
		for (c = 0; c < n; c++) {

			printf("%d ", sorted[c]);

		}

		printf("\n");
		printf("\n");*/
		stop = MPI_Wtime();
		free(sorted);
		free(other_array);

	}

	free(original_array);
	free(sub_array);
	free(tmp_array);
	if (world_rank == 0) printf("%f\n", stop - start);

	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

}


void merge(int *a, int *b, int l, int m, int r) {

	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;

	while ((h <= m) && (j <= r)) {
		if (a[h] <= a[j]) {
			b[i] = a[h];
			h++;
		}
		else {
			b[i] = a[j];
			j++;
		}
		i++;
	}

	if (m < h) {
		for (k = j; k <= r; k++) {
			b[i] = a[k];
			i++;
		}
	}

	else {
		for (k = h; k <= m; k++) {
			b[i] = a[k];
			i++;
		}
	}

	for (k = l; k <= r; k++) {
		a[k] = b[k];
	}
}

void mergeSort(int *a, int *b, int l, int r) {
	int m;
	if (l < r) {
		m = (l + r) / 2;
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
	}
}