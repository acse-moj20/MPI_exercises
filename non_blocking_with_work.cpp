/*
 Lecture 2 Exercise 2: Non Blocking Communications w/ Work Completion During Wait
 =================================================================================

Rewrite the example on point to point communication but now include sending of an array of
10,000 doubles. Write a function to perform some calculations and repeatedly call this function.

Print to screen the number of cycles of task were completed by each process.

*/

#include <mpi.h>
#include <iostream>
#include <time.h>
#include <vector>


using namespace std;
int id, p;
int num_recv_procs = 0;
std::vector<int> send_recv_procs;

void do_Work(void) {
	
	int summ = 0;
	for (int i = 0; i < 100; i++)
		summ += 7 + rand() % p;
}


int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	int tag_num = 1;
	int work_cnt = 0;
	int sent_num = 10000;
	

	int cnt = 0, flag = 0;


	MPI_Request* request = new MPI_Request[(p - 1) * 2];
	double** recv_data = new double* [p - 1];
	double** send_data = new double* [p - 1];
		
		
	for (int i = 0; i < p; i++) {
		if (i != id) {
			send_recv_procs.push_back(i);
		}
	}

	for (int i = 0; i < p - 1; i++) {
		send_data[i] = new double[sent_num];
		for (int j = 0; j < sent_num; j++) {
			send_data[i][j] = rand() %p;
		}
		
		MPI_Isend(send_data[i], sent_num, MPI_DOUBLE, send_recv_procs[i], tag_num, MPI_COMM_WORLD, &request[cnt]);
		cnt++;
		
		recv_data[i] = new double[sent_num];
		MPI_Irecv(recv_data[i], sent_num, MPI_DOUBLE, send_recv_procs[i], tag_num, MPI_COMM_WORLD, &request[cnt]);
		cnt++;
		
	}
	while (MPI_Testall((p-1) * 2, request, &flag, MPI_STATUSES_IGNORE) == MPI_SUCCESS && flag == 0) {
		do_Work();
		work_cnt++;
	}
	

	MPI_Waitall(cnt, request, MPI_STATUSES_IGNORE);

	delete[] send_data;
	delete[] recv_data;
	delete[] request;

	cout << "Processor " << id << " did " << work_cnt << " cycles while waiting." << endl;
	cout.flush();
	

	
	MPI_Finalize();

	return 0;
}