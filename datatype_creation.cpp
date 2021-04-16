#include <mpi.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <vector>

/*
  Lecture 4 Exercise 1 : Creating MPI Datatype for a Lagrangian Particle
  =======================================================================

 When carrying out Lagrangian simulations it is useful to have a class that stores the position of a point/particle 
 (as well as all its other properties). 
 
 Create a class to store a 2D position and velocity. 
 Choose a maximum vertical and horizontal extent for your domain and then create 10 000 
 randomly located particles within these extents on processor zero. Divide the domain up into vertical stripes 
 of equal width so that there can be a domain assigned to each of the processes.

 Create an MPI type that can send all the information in an object of the class’ type together. 
 Send the particles individually from the root to the appropriate processor according to its horizontal position. 
 This can be done using either blocking or non-blocking sends and receives. You can send an empty communication 
 to indicate that all the particles have been sent.

*/
int id, p;

// class of Lagragian point/particle. Stores position and velocity of particle.
class Particle
{
public:
	double position[2]; // x and y position of particle.
	double v[2];        // x and y velocity of particle.

	static MPI_Datatype MPI_Type;
	static void create_type(void);
};

MPI_Datatype Particle::MPI_Type; // data type to be created

// generates the custom data type for class Particle
void Particle::create_type()
{
	// parameters to define the data type in memory

	int block_lengths[2];			// size of blocks in memory for variable
	MPI_Aint displacements[2];		
	MPI_Aint addresses[2], add_start;
	MPI_Datatype typelist[2];

	Particle temp;		// temp object of class Particle

	typelist[0] = MPI_DOUBLE;
	block_lengths[0] = 2;
	MPI_Get_address(&temp.position, &addresses[0]);  // gets adress of particle's position and stores in address list

	typelist[1] = MPI_DOUBLE;
	block_lengths[1] = 2;
	MPI_Get_address(&temp.v, &addresses[1]);  // gets adress of particle's position and stores in address list
 
	MPI_Get_address(&temp, &add_start);
	for (int i = 0; i < 2; i++) { displacements[i] = addresses[i] - add_start; }

	MPI_Type_create_struct(2, block_lengths, displacements, typelist, &MPI_Type);
	MPI_Type_commit(&MPI_Type);

}


const double position_max[2] = { 1.0, 1.0 };
const double v_max[2] = { 1.0, 1.0 };
std::vector<Particle> particle_list;

// generates random double strictly less than one
double random_no() {
	return ((double)rand()) / ((double)RAND_MAX + 1.0);
}

int proc_from_x(double* x)
{
	return (int)((x[0]) * p) / position_max[0];
}

void create_send_particles() {

	// generating randomly located particles with max x,y value of [1.0, 1.0] and sending to processors
	for (int i = 0; i < 10000; i++) {
		Particle temp;
		temp.position[0] = random_no() * position_max[0];
		temp.position[1] = random_no() * position_max[1];
		temp.v[0] = 0.0;
		temp.v[1] = 0.0;

		int dest = proc_from_x(temp.position);

		if (dest == 0) {
			particle_list.push_back(temp);
		}
		else {
			MPI_Send(&temp, 1, Particle::MPI_Type, dest, 0, MPI_COMM_WORLD);
		}
	}

	for (int i = 1; i < p; i++) {
		MPI_Send(nullptr, 0, Particle::MPI_Type, i, 0, MPI_COMM_WORLD);
	}
}

void receive_particles(void) {
	
	MPI_Status status;
	Particle recv_particle;


	do {
		
		int particle_count;
		MPI_Recv(&recv_particle, 1, Particle::MPI_Type, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, Particle::MPI_Type, &particle_count);
		
		if (particle_count == 1) {
			particle_list.push_back(recv_particle);
		}
		else if (particle_count == 0) {
			break;
		}
		else {
		std::cout << "Unexpected number of particles was received!" << std::endl;
		break;
		}
	} 
	while (true);
	
}

int main(int argc, char *argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	Particle::create_type();

	if (id == 0) {
		create_send_particles();
	}
	else {
		receive_particles();
	}
	std::cout <<"Process "<< id << " : is responsible for " << particle_list.size() << " particles" << std::endl;

	MPI_Type_free(&Particle::MPI_Type);
	MPI_Finalize();

	return 0;
}