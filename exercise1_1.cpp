#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>

/*
Lecture 1 Exercise 1 : Ring Communication
==========================================

Write a program that does a ring communication.Each process should send 
a new number to the next process in addition to the numbers received from 
the previous process.In other words, process zero should send a single number 
to process 1, process 1 should send two numbers to process 2 (the original one 
from the first process together with a new number), process 2 should then 
send 3 numbers to process 3 etc..This should continue until the last process 
sends the entire list of numbers back to the first process.At each stage the 
process should give its idand the number that it is adding to the list, with process 
zero then outputting the entire list when it gets back to it.Note that each process 
should do a single sendand a single receive, with an array being sent between the processes.

*/

using namespace std;
int id, p;

int main(int argc, char* argv[])
{

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 100 );

	int tag_num = 1;
	int* data = new int[p];

	if (id == 0) {
		// data to send 
		
		data[0] = rand();

		MPI_Send(data, 1, MPI_INT, 1, tag_num, MPI_COMM_WORLD);
		cout << "Processor " << id << " added " << data[0] << endl;
		MPI_Recv(data, p, MPI_INT, p - 1, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		cout << "Receiving all these back to processor : " << id << endl;
		for (int j = 0; j != p; j++) {
			cout << data[j] << "  ";
		}
		cout << endl;
		cout.flush();
		tag_num++;

		delete[] data;
	}
	else {
		int id_recv = id - 1;
		int id_send = (id + 1) % p;
		int data_count = id + 1;
		MPI_Recv(data, data_count - 1, MPI_INT, id_recv, tag_num, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

		data[id] = rand();
		MPI_Send(data, data_count, MPI_INT, id_send, tag_num, MPI_COMM_WORLD);
		cout << "Processor " << id << " added " << data[data_count - 1] << endl;
		cout.flush();
		tag_num++;

		delete[] data;

	}
	MPI_Finalize();

}
/*

written by Mustapha Jolaade
March 10, 2021

*/