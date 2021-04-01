#include <mpi.h>
#include <iostream>
#include <time.h>
#include <cstdlib>


using namespace std;
int id, p;

/*
 Lecture 1 Exercise 3 : Blocking Point to Point Communication
 =============================================================

Write a program where every processor sends a piece of information to every 
other processor using MPI_Send and MPI_Recv.

a. The easiest, but slowest way to do this is to allow each process to have a turn to send data, 
	with each other process waiting for that data (if you have 100 processes they will by waiting 99% 
	of the time and only communicating 1% of the time on average). Implement this method.

b. It is much quicker to allow every process to be sending or receiving data at the same time. 
	You can either achieve this with clever ordering of the sends and receives or by using probe 
	to handle communications as they come in (though you still need to watch out for potential 
	blocking – We will be looking at non-blocking communications later. These are a bit more complex 
	to implement, but remove the need to

c. Order communications to avoid blocking. See if you can implement this using blocking communications. 
   Don’t worry if it crashes when you first try, it is easy to get something wrong here!


*/

int main(int argc, char* argv[])
{
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	int tag_num = 1;

	// [Implementation (a)]: each process takes a turn at sending to other processors. 
	// This is the slowest but easiest way of P2P blocking communication.

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
	MPI_Finalize();
	return 0;
}