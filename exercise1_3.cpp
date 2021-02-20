#include <mpi.h>
#include <iostream>
#include <time.h>
#include <cstdlib>

using namespace std;

int id, p;

// Lecture 1 Exercise 3 : Blocking Point to Point Communication
// =============================================================
int main(int argc, char* argv[])
{
	// the goal is to have every processor speaking to every other processor.
	// there are 3 different implementations of this going form basic to advanced ordering of communications.

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	int tag_num = 1;

#if 0

	// [Implementation A]: each process takes a turn at sending to other processors. 
	// This is the slowest but easiest way of P2P communication.

	for (int s_id = 0; s_id != p; s_id++)
	{
		if (s_id == id) {
			
			for (int j = 0; j < p; j++) {
				if (j != id) {
					double send_data = rand()% 10;		// Data to be sent. Random double.
					MPI_Send(&send_data, 1, MPI_DOUBLE, j, tag_num, MPI_COMM_WORLD);

				}
			}

		}
		else {
			double data;
			MPI_Recv(&data, 1, MPI_DOUBLE, s_id, tag_num, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			cout << "Processor " << id << " received " << data << " from processor: " << s_id << endl;

		}
		tag_num++;
	}
#elif 1
	// [Implementation B]: Each process sending and receiving simultaneously. 
	// Invovles ordering of sends/receives or using probes to handle communication.

	int mid = p / 2;
	if (id < mid)
	{
	// Send data to another processor first
		for (int s_id = p-1; s_id >=mid; s_id--)
		{
			double send_data = double(s_id);
			MPI_Send(&send_data, 1, MPI_DOUBLE, s_id, tag_num, MPI_COMM_WORLD);
			cout << "Processor: " << id << " sent to Processor: " << s_id << endl;
			double recv_data;
			MPI_Recv(&recv_data, 1, MPI_DOUBLE, s_id, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//cout << "Processor: " << id << " received from Processor: " << s_id << endl;
			cout.flush();
		
		}
		
	}
	else {
		// Receive data from  another processor first
		for (int s_id = 0; s_id < mid ; s_id++)
		{
			if (s_id != id) {

				int num_recv;				// number of data to be recievied.
				MPI_Status status;
				auto c = MPI_Probe(s_id, tag_num, MPI_COMM_WORLD, &status);
				auto d = MPI_Get_count(&status, MPI_DOUBLE, &num_recv);
				

				double recv_data;
				MPI_Recv(&recv_data, 1, MPI_DOUBLE, s_id, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				//cout << "Processor: " << id << " received from Processor: " << s_id << endl;
				double send_data = double(s_id) / double(p);
				MPI_Send(&send_data, 1, MPI_DOUBLE, s_id, tag_num, MPI_COMM_WORLD);
				cout << "Processor: " << id << " sent to Processor: " << s_id << endl;
				cout.flush();
			}
		}
		
	}

	tag_num++;
#else 
	// [Implementation C]: 

#endif
	MPI_Finalize();
}