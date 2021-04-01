#include <mpi.h>
#include <iostream>
#include <time.h>
#include <cstdlib>


using namespace std;
int id, p;
int tag_num = 1;

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

void comm_range(int bottom, int top)
{
	if (top == bottom || id<bottom || id>top)
		return;

	int range = (top - bottom) / 2 + 1;
	int mid = bottom + range;

	if (id < mid)
	{
		int offset = id - bottom;

		for (int i = 0; i < range; i++)
		{
			int other_proc = (offset + i) % range + mid;

			if (other_proc <= top)
			{
				int send_data = id;
				MPI_Send(&send_data, 1, MPI_INT, other_proc, tag_num, MPI_COMM_WORLD);
				//cout << "Process " << id << " sent " << send_data << " to process " << other_proc << endl;
				int recv_data;
				MPI_Recv(&recv_data, 1, MPI_INT, other_proc, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				cout << "Process " << id << " received " << recv_data << " from process " << other_proc << endl;
				cout.flush();
			}
		}
	}
	else
	{
		int offset = id - mid;

		for (int i = 0; i < range; i++)
		{
			int other_proc = (offset - i + range) % range + bottom;
			int recv_data;
			MPI_Recv(&recv_data, 1, MPI_INT, other_proc, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout << "Process " << id << " received " << recv_data << " from process " << other_proc << endl;
			int send_data = id;
			MPI_Send(&send_data, 1, MPI_INT, other_proc, tag_num, MPI_COMM_WORLD);
			//cout << "Process " << id << " sent " << send_data << " to process " << other_proc << endl;
			cout.flush();
		}
	}

	comm_range(bottom, mid - 1);
	comm_range(mid, top);
}

int main(int argc, char* argv[])
{

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	

	// [Implementation b & c]: Each process sending and receiving simultaneously. 
	// Invovles ordering of sends/receives or using probes to handle communication.

	comm_range(0, p - 1);

	tag_num++;
	MPI_Finalize();
	return 0;
}