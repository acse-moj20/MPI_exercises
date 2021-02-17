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


	// [Implementation A]: each process takes a turn at sending to other processors. 
	// This is the slowest but easiest way of P2P communication.

	for (int s_id = 0; s_id != p; s_id++)
	{
		if (s_id == id) {
			
			for (int j = 0; j < p; j++) {
				if (j != id) {
					double send_data = rand()% 10;		// Data to be sent. Random double.
					MPI_Send(&send_data, 1, MPI_INT, j, tag_num, MPI_COMM_WORLD);

				}
			}

		}
		else {
			double data;
			MPI_Recv(&data, 1, MPI_INT, s_id, tag_num, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			cout << "Processor " << id << " received " << data << " from processor: " << s_id << endl;

		}
		tag_num++;
	}

	MPI_Finalize();
}