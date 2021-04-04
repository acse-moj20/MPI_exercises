#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>


int id, p;

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	int tag_num = 1;
	int* data[10000];

	if (id == 0) {

	}


	MPI_Finalize();

	return 0;
}