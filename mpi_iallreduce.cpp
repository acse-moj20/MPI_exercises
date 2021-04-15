/*
 Lecture 3 Exercises 2 - 4: MPI_REDUCE, MPI_ALLREDUCE, MPI_IALLREDUCE
 =====================================================================

Have each process calculate a random “time step”and then use a combination of MPI_Reduce
and MPI_Bcast to ensure that every process knows the same smallest time step.

Exercise 3: MPI_Allreduce

Use MPI_Allreduce to combine the MPI_Reduce and MPI_Bcast in the previous example into a single operation.

Exercise 4 : MPI_Iallreduce

Modify the code from the previous exercise to use a non - blocking MPI_Iallreduce rather than the blocking MPI_Allreduce.
In this example there is no real advantage to using a non - blocking collective communication, 
but in other situations it may be worth doing so in order to allow other calculations to be carried out while waiting 
for the communications to complete.
*/

#include <mpi.h>
#include <iostream>
#include <time.h>
#include <cstdlib>

int id, p;

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	double time_step = (double)rand() / RAND_MAX;
	double recv_data;
	MPI_Request* request;

	std::cout << " Time step of process " << id << " is: " << time_step << std::endl;
	MPI_Iallreduce(&time_step, &recv_data, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD, request);

	MPI_Wait(request, MPI_STATUS_IGNORE);
	
	std::cout << " Process " << id << " has minimum time step of: " << recv_data << std::endl;

	MPI_Finalize();
	return 0;
}

