#include <mpi.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include "Exercises.h"

using namespace std;

int chunk_size = 5;

// this is equivalent to a gather 

int exercise5_1(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	int* send_data = nullptr;
	int* recv_data = nullptr;
	MPI_Win window;

	//only process zero needs remotely accessible memory in this example
	MPI_Win_allocate(sizeof(int) * chunk_size, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &send_data, &window);

	if (id == 0) {
		recv_data = new int[chunk_size * p];
	}
	cout << "Data sent by process " << id << ": ";
	for (int i = 0; i < chunk_size; i++) {
		send_data[i] = rand() % 1000;
		cout << send_data[i] << "\t";

	}
	cout << endl;
	
	// MPI_Win_fence(MPI_MODE_NOPRECEDE, window);
	MPI_Group comm_group;
	MPI_Group group;
	int* group_ids = nullptr;

	MPI_Comm_group(MPI_COMM_WORLD, &comm_group);

	if (id == 0)
	{

		group_ids = new int[p];
		for (int n = 1; n < p; n++) {
			group_ids[n - 1] = n;
		}
		MPI_Group_incl(comm_group, p - 1, group_ids, &group);

		MPI_Win_start(group, 0, window);
		for (int i = 0; i < p; i++)
		{
			MPI_Get(&recv_data[i * chunk_size], chunk_size, MPI_INT, i, 0, chunk_size, MPI_INT, window);
		}

		for (int i = 0; i < chunk_size; i++) {
			recv_data[i] = send_data[i];
		}

		MPI_Win_complete(window);
	}
	else {
		group_ids = new int[1];
		group_ids[0] = 0;
		MPI_Group_incl(comm_group, 1, group_ids, &group);

		MPI_Win_post(group, 0, window);
		MPI_Win_wait(window);
	}


	if (id == 0)
	{
		for (int i = 0; i < p; i++) {
			cout << "Data sent by process " << i << ": ";
			for (int n = 0; n < chunk_size; n++) {
				cout << recv_data[i * chunk_size + n] << "\t";
			}
			cout << endl;
		}
		
	} 

	MPI_Win_free(&window);

	delete[] recv_data;

	MPI_Finalize();

	return 0;
}
