#include <mpi.h>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

int id, p;

// Lecture 1 Exercise 2: Ring Communication w/Probe 
// ================================================
int exercise1_2(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);	

	
	int tag_num = 1;

	if (id == 0) {
		// data to send 

		int num_recv;
		int num_send = 1 + rand() % 3;
		int* data = new int[num_send];

		for (int j = 0; j < num_send; j++) { data[j] = rand(); }

		MPI_Send(data, num_send, MPI_INT, id + 1, tag_num, MPI_COMM_WORLD);
		cout << "Processor " << id << " added: ";
		for (int i = 0; i != num_send; i++) {
			cout << data[i] << " ";
		}
		cout << endl;

		MPI_Status status;
		MPI_Probe(p - 1, tag_num, MPI_COMM_WORLD, &status);		// Probe 
		MPI_Get_count(&status, MPI_INT, &num_recv);
		MPI_Recv(data, num_recv, MPI_INT, p - 1, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << "Receiving all these info back: ";
		for (int j = 0; j != num_recv; j++) {
			cout << data[j] << " ";
		}
		cout << " to processor " << id << ".";
		tag_num++;

		delete[] data;
	}
	else {
		int id_recv = id - 1;
		int id_send = (id + 1) % p;
		int num_recv;
		int num_send = 1 + rand() % 3;

		MPI_Status status;
		MPI_Probe(id_recv, tag_num, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &num_recv);
		int* data = new int[num_recv];
		MPI_Recv(data, num_recv, MPI_INT, id_recv, tag_num, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

		for (int j = num_recv; j < num_recv + num_send; j++) { data[j] = rand(); }

		MPI_Send(data, num_recv + num_send, MPI_INT, id_send, tag_num, MPI_COMM_WORLD);
		cout << "Processor " << id << " added: ";
		for (int i = num_recv; i != num_recv + num_send; i++) {
			cout << data[i] << " ";
		}
		cout << endl;
		tag_num++;

		delete[] data;

	}


	MPI_Finalize();
}