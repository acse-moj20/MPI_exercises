#include <mpi.h>
#include <iostream>
#include <time.h>
#include <vector>


using namespace std;

int id, p;
int num_recv_procs = 0;
int* recv_procs = new int[num_recv_procs];
int* send_procs = new int[2];


void do_Comms(void) {

	int tag_num = 1;
	MPI_Request* request = new MPI_Request[(p - 1) * 2];
	double send_data[2];
	double* recv_data = new double[num_recv_procs];

	
	int cnt = 0;
	for (int i = 0; i < num_recv_procs; i++) {
		MPI_Irecv(&recv_data[i], 1, MPI_DOUBLE, recv_procs[i], tag_num, MPI_COMM_WORLD, &request[cnt]);
		cnt++;
	}

	for (int i = 0; i < 2; i++) {
		send_data[i] = id * (i + 1) / 100.0;
		MPI_Isend(&send_data[i], 1, MPI_DOUBLE, send_procs[i], tag_num, MPI_COMM_WORLD, &request[cnt]);
		cnt++;
	}
	MPI_Waitall(cnt, request, MPI_STATUSES_IGNORE);

	
	cout << "Processor " << id << " sending ";
	for (int i = 0; i < 2; i++) {
		cout << send_data[i] << " to: " << send_procs[i] << "\t";
	}

	cout << "receiving: ";
	for (int i = 0; i < num_recv_procs; i++) {
		cout << recv_data[i] << "\t"
			<< " from: " << recv_procs[i];
	}

	cout << endl;
	cout.flush();
	delete[] recv_data;
	delete[] request;

}



// Lecture 2 Exercise 1: Non Blocking P2P Communication
// ====================================================

int exercise2_1(int argc, char* argv[]) {

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
	MPI_Waitall(cnt, request, MPI_STATUSES_IGNORE);
	
	cout << "Processor " << id << " sending to: ";
	for (int i = 0; i < p; i++) {
		if (send_status[i]) { cout << i << "\t"; }
	}
	cout << "receiving from: ";
	for (int i = 0; i < p; i++) {
		if (recv_status[i]) {
			cout << i << "\t";
			num_recv_procs++;
			recv_procs[num_recv_procs] = i;
		}
		cout << "\n" << id << " receiving " << num_recv_procs << " processes from: " << recv_procs[num_recv_procs];
	}
	cout << endl;
	cout.flush();
	


	//for (int i = 0; i < 10; i++) {
	//do_Comms();
	//}

	delete[] send_status;
	delete[] recv_status;
	delete[] request;
	MPI_Finalize();
}

