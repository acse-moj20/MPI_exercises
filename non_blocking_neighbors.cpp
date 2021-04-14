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
vector<int> send_procs;
vector<int> recv_procs;
int id, p, m, n;


void setup_Comms(void) {
	int tag_num = 1;
	MPI_Request* request = new MPI_Request[(p - 1) * 2];

	bool* send_status = new bool[p];
	bool* recv_status = new bool[p];
	for (int i = 0; i != p; i++) { send_status[i] = false; }

	// Calculating i, j index of id
	int index_i = id % m; // column index of process
	int index_j = id / m; // row index of process
	
	
	for (int send_id = 0; send_id < p; send_id++) {
		int i = send_id % m;
		int j = send_id / m;

		// Setting up communication to neighbors based on column and row indices.

		if (((i == index_i + 1 || i == index_i - 1) && (j == index_j || j == index_j - 1 || j == index_j + 1)) ||
			(i == index_i && (j == index_j - 1 || j == index_j + 1))) {
			
			send_procs.push_back(send_id);
			recv_procs.push_back(send_id);
			send_status[send_id] = true;
			recv_status[send_id] = true;
		}

	} 

	int cnt = 0;
	// we set up all communications then do some potential calculations while waiting. All communications are checked afterwards for completion. 
	for (int i = 0; i < p; i++) {
		if (i != id) {
			MPI_Irecv(&recv_status[i], 1, MPI_C_BOOL, i, tag_num, MPI_COMM_WORLD, &request[cnt]);
			cnt++;
			MPI_Isend(&send_status[i], 1, MPI_C_BOOL, i, tag_num, MPI_COMM_WORLD, &request[cnt]);
			cnt++;
		}
		else {
			recv_status[i] = false;
		}


	}

	MPI_Waitall(cnt, request, MPI_STATUSES_IGNORE);


	cout << "Processor " << id << " sending to: ";
	for (int i = 0; i < p; i++) {
		if (send_status[i]) { cout << i << " "; }
	}
	cout << endl;
	cout.flush();
}

int main(int argc, char* argv[]) {


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	n = sqrt(p);
	m = p / n;
	if (m * n != p) { n -= 1; m = p / n; }

	//cerr << m << " " << n << endl;
	setup_Comms();


	MPI_Finalize();
	return 0;
}