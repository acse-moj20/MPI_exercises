// Lecture 2 Exercise 1: Non Blocking P2P Communication
// ====================================================
/* Notes:
	1. request variable has the information about a particular communication 
	2. since we have a communication that is not copleting yet.
	3. this section only sets up the communication. It doesnt wait for the communication to be completed.
	4. it is only after the wait all that we can check that all communications have been completed.
	5. You cant also reuse the variable that is being sent while in the communications set up. You can only re use it after the wait. 
	6. It is perfectly acceptable to mix and match a blocking send/receive with a non-blocking send/receive.

*/
#include <mpi.h>
#include <iostream>
#include <time.h>
#include <vector>
#include "Exercises.h"

using namespace std;
int id, p;
int num_recv_procs = 0;
std::vector<int> recv_procs;
std::vector<int> send_procs;

// Completes the non-blocking communication following our setup.
void do_Comms(void) {

	int tag_num = 1;
	MPI_Request* request = new MPI_Request[num_recv_procs + 2];
	double send_data[2];
	double* recv_data = new double[num_recv_procs];

	
	int cnt = 0;
	for (int i = 0; i < num_recv_procs; i++) {
		MPI_Irecv(&recv_data[i], 1, MPI_DOUBLE, recv_procs[i], tag_num, MPI_COMM_WORLD, &request[cnt]); // this version of MPI receive is the non blocking version.
		cnt++;
	}

	for (int i = 0; i < 2; i++) {
		send_data[i] = id * (i + 1) / 100.0; // data to be sent.
		MPI_Isend(&send_data[i], 1, MPI_DOUBLE, send_procs[i], tag_num, MPI_COMM_WORLD, &request[cnt]); // this version of MPI send is the non blocking version.
		cnt++;
	}
	MPI_Waitall(cnt, request, MPI_STATUSES_IGNORE);
	tag_num++;
	
	cout << "Processor " << id << " sending ";
	for (int i = 0; i < 2; i++) {
		cout << send_data[i] << " to: " << send_procs[i] << ", ";
	}

	cout << endl;
	cout.flush();
	delete[] recv_data;
	delete[] request;

}


int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	int tag_num = 1;
	MPI_Request* request = new MPI_Request[(p-1) * 2];

	bool *send_status = new bool [p];
	bool *recv_status = new bool[p];
	for (int i = 0; i != p; i++) { send_status[i] = false;}

	// Setting up communications
			// array of randomly generated processor to send to 
	int* send_id = new int[2];
	send_id[0] = rand() % p;
	send_id[1] = rand() % p;

	// making sure processor isnt sending to itself or the same 2 randomly selected processors.
	while (send_id[0] == id) { send_id[0] = rand() % p; }
	while (send_id[0] == send_id[1] || send_id[1] == id) { send_id[1] = rand() % p; }

	send_status[send_id[0]] = true;
	send_status[send_id[1]] = true;
	send_procs.push_back(send_id[0]);
	send_procs.push_back(send_id[1]);

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
	
	cout << " \treceiving from: ";
	for (int i = 0; i < p; i++) {
		if (recv_status[i]) {
			cout << i << "\t";
		}
	}
	cout << endl;
	cout.flush();

	for (int i = 0; i < p; i++) {
		if (recv_status[i]) {
			num_recv_procs++;
			recv_procs.push_back(i);
		}
	}

	for (int i = 0; i < 10; i++) {
	do_Comms();
	}

	delete[] send_status;
	delete[] recv_status;
	delete[] request;
	MPI_Finalize();

	return 0;
}

