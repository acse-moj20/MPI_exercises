#include <mpi.h>
#include <iostream>

using namespace std;



int mpidemo(int argc, char* argv[])
{
	int id, p;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	cout << "Processor " << id << " of " << p << endl;
	cout.flush();

	MPI_Finalize();

}