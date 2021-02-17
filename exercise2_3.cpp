#include <mpi.h>
#include <iostream>
#include <time.h>
#include <vector>


int id, p;

// Lecture 2 Exercise 3: Non-blocking Communications with Neighbours
// ==================================================================


// Setting up communications
// array of randomly generated processor to send to 
void comms_setup(void) {
	int tag_num = 1;
	MPI_Request* request = new MPI_Request[(p - 1) * 2];

	int recv_size = 0;
	if (id % 2 == 0)
	bool* send_status = new bool[p];
	bool* recv_status = new bool[p];
	for (int i = 0; i != p; i++) { send_status[i] = false; }

	
	int* send_id = new int[2];
	send_id[0] = rand() % p;
	send_id[1] = rand() % p;
	while (send_id[0] == id) { send_id[0] = rand() % p; }
	while (send_id[0] == send_id[1] || send_id[1] == id) { send_id[1] = rand() % p; }
	send_status[send_id[0]] = true;
	send_status[send_id[0]] = true;
	send_procs[0] = send_id[0];
	send_procs[1] = send_id[1];

	int cnt = 0;
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

}




int main(int argc, char* argv[]) {


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);




	MPI_Finalize();
}