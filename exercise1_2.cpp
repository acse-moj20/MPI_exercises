#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>

/*
 Lecture 1 Exercise 2: Ring Communication w/Probe 
 ================================================

 Modify the Ring Communication program from exercise 1 so that each process 
 randomly adds between 1 and 3 new numbers to the list being passed around the ring. 
 You should probe for the size of the data to be received, allocate enough memory to 
 store both this data and the new data to be added to the list, receive this data, 
 append the new data to the end of the dynamically allocated array and then send this
 data on to the next processor. Remember to free the memory after using it.

*/

using namespace std;
int id, p;

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);	

	
	int tag_num = 1;
	int* data;	// initialize placeholder for data to send

	if (id == 0) {
		// data to send 

		int recv_count;
		int send_count = 1 + rand() % 3; // send between 2 to 3 integers
		data = new int[send_count];	// 

		cout << "Processor " << id << " added: ";
		for (int j = 0; j < send_count; j++) { 
			data[j] = rand(); 
			cout << data[j] << ' ';
		}
		cout << endl;
		MPI_Send(data, send_count, MPI_INT, id + 1, tag_num, MPI_COMM_WORLD);
		delete[] data;	// clear data after sending

		MPI_Status status;
		MPI_Probe(p - 1, tag_num, MPI_COMM_WORLD, &status);		// Probe to get status
		MPI_Get_count(&status, MPI_INT, &recv_count);			// Get count then create data of same size
		data = new int[recv_count];
		MPI_Recv(data, recv_count, MPI_INT, p - 1, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		cout << "Receiving all these info back: ";
		for (int j = 0; j != recv_count; j++) {
			cout << data[j] << " ";
		}
		cout << " to processor " << id << ".";
		tag_num++;

		delete[] data;
	}
	else {
		int id_recv = id - 1;
		int id_send = (id + 1) % p;
		int recv_count;
		int send_count = 1 + rand() % 3; // additional data sent by processor

		MPI_Status status;
		MPI_Probe(id_recv, tag_num, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &recv_count);

		data = new int[recv_count + send_count];
		MPI_Recv(data, recv_count, MPI_INT, id_recv, tag_num, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		for (int j = recv_count; j < recv_count + send_count; j++) { 
			data[j] = rand(); }

		MPI_Send(data, recv_count + send_count, MPI_INT, id_send, tag_num, MPI_COMM_WORLD);
		cout << "Processor " << id << " added: ";
		for (int i = recv_count; i != recv_count + send_count; i++) {
			cout << data[i] << " ";
		}
		cout << endl;
		tag_num++;

		delete[] data;

	}


	MPI_Finalize();

	return 0;
}