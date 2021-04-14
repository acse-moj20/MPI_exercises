#include <mpi.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <cmath>

/*
 Lecture 2 Exercise 3: Non-blocking Communications with Neighbours
 ==================================================================

 Divide your p processes into a grid of size m x n (try and ensure that m and n are integers that are as close to one another as possible.
 E.g. if p is 9, m and n should both be 3, while if p is 12, one should be 3 and the other 4).
 On this grid calculate an i and j index for each process such that id = i + m j

Each process should communicate with the processes that next to it vertically, horizontally and diagonally
(i.e.processes in the middle of the grid will communicate with 8 neighbours, 
those on edges with 5 neighbours and those in the corners with 3 neighbours).
Send the neighbours the source’s id as well as their i and j coordinates and display these

Communication patterns similar to this are also quite commonly used.
You might encounter it if you are doing peer to peer communication for a domain decomposition problem (more on this later).
*/


using namespace std;
vector<int> send_recv_procs;
int num_send_recv = 0;
int id, p, m, n;
int index_i; // column index of process
int index_j; // row index of process

void setup_Comms(void) {
	

	for (int send_id = 0; send_id < p; send_id++) {
		int i = send_id % m;
		int j = send_id / m;

		// Setting up communication to neighbors based on column and row indices.

		if (((i == index_i + 1 || i == index_i - 1) && (j == index_j || j == index_j - 1 || j == index_j + 1)) ||
			(i == index_i && (j == index_j - 1 || j == index_j + 1))) {

			send_recv_procs.push_back(send_id);
			num_send_recv++;
		}

	}
}

void do_Comms(void){
	int cnt = 0;
	int tag_num = 1;

	int* send_data = new int[num_send_recv * 3];
	int* recv_data = new int[num_send_recv * 3];
	MPI_Request* request = new MPI_Request[num_send_recv * 2];


	for (int i = 0; i < num_send_recv; i++) {
		send_data[cnt * 3] = id;
		send_data[cnt * 3 + 1] = index_j;
		send_data[cnt * 3 + 2] = index_i;
		
		MPI_Irecv(&recv_data[i*3], 3, MPI_INT, send_recv_procs[i], tag_num, MPI_COMM_WORLD, &request[cnt * 2]);
		MPI_Isend(&send_data[i*3], 3, MPI_INT, send_recv_procs[i], tag_num, MPI_COMM_WORLD, &request[cnt * 2 + 1]);
		cnt++;


	}

	MPI_Waitall(cnt*2, request, MPI_STATUSES_IGNORE);


	cout << id << " receiving from ";
	for (int i = 0; i < cnt; i++) {
		cout << recv_data[i * 3] << " ( " << recv_data[i * 3 + 1] << ", " << recv_data[i * 3 + 2] << ")\t";
	}
	cout << endl;
	cout.flush();

	delete[] send_data;
	delete[] recv_data;
	delete[] request;
}

int main(int argc, char* argv[]) {


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	n = sqrt(p);
	m = p / n;
	if (m * n != p) { n -= 1; m = p / n; }

	index_i = id % m; // column index of process
	index_j = id / m; // row index of process

	//cerr << m << " " << n << endl;
	setup_Comms();
	do_Comms();

	MPI_Finalize();
	return 0;
}