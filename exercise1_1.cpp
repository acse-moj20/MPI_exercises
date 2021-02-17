#include <mpi.h>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

int id, p;

// Lecture 1 Exercise 1 : Ring Communication
// ================================
int main(int argc, char* argv[])
{

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	int tag_num = 1;

	if (id == 0) {
		// data to send 
		int* data = new int[p];
		data[0] = rand();

		MPI_Send(data, 1, MPI_INT, id + 1, tag_num, MPI_COMM_WORLD);
		

		MPI_Recv(data, p, MPI_INT, p - 1, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << "Receiving all these back: ";
		for (int j = 0; j != p; j++) {
			cout << data[j] << "  ";
		}
		cout << " to processor: " << id << " " << endl;
		tag_num++;

		delete[] data;
	}
	else {
		int id_recv = id - 1;
		int id_send = (id + 1) % p;
		int *data = new int [id];
		MPI_Recv(data, id, MPI_INT, id_recv, tag_num, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

		cout << "Receiving " << data[id -1] << " on processor: " << id << endl;


		data[id] = rand();

		MPI_Send(data, id + 1, MPI_INT, id_send, tag_num, MPI_COMM_WORLD);
		// cout << "Sending " << data[id] << " to processor: " << id_send << endl;
		tag_num++;

		delete[] data;

	}
	MPI_Finalize();
}