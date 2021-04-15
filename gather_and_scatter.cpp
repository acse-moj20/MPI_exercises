/*
 Lecture 3 Exercise 1: Gather and Scatter Operations
 ====================================================

Have every process calculate a single random number between 0 and 100. 
Later on we wish to communicate based on the order of these numbers (you don’t need to implement this communication).

You now need to ensure that every process knows what this order is. 
In other words, the process with the lowest random number should be first in the list, the one with the second lowest second etc. 
(if they have the same number you should order them according to the process id).

One tactic to do this is to gather all of the random numbers onto a single process, sort the list while 
keeping track of the id corresponding to each number, using any sort algorithm you choose (bubble sort is very easy to implement, 
while quick sort is much quicker, but a bit trickier to do. You could alternatively just rely on the sort algorithm from std, 
though you will need think about getting the indexes in order). 

Scatter the list of id order back to all the processes (i.e. send each one there number in the sorted order).

*/

#include <mpi.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;
int id, p;

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	int id_number = rand() % 101; // random number of id
	int* num_list;
	int* temp_list;
	int* id_list;
	int* send_list;
	
	
	if (id == 0) {

		num_list = new int[p];
		id_list = new int[p];
		temp_list = new int[p];
		send_list = new int[p];
	}

	MPI_Gather(&id_number, 1, MPI_INT, num_list, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	if (id == 0) {
		
		

		for (int i = 0; i < p; i++) {
			id_list[i] = i;
			temp_list[i] = num_list[i];
		}
		std::sort(temp_list, temp_list + p);

		for (int i = 0; i < p; i++) {
			
			for (int j = 0; j < p; j++) {
			 if (temp_list[i] == num_list[j]) { id_list[i] = j; }
			}

			send_list[id_list[i]] = i;
		
		}
	}

	int id_order;
	MPI_Scatter(send_list, 1, MPI_INT, &id_order, 1, MPI_INT, 0, MPI_COMM_WORLD);
	cout << "Process " << id << " is:   " << id_order << " in list. Priority No is: " << id_number << endl;

	

	if (id == 0) {
		delete[] num_list;
		delete[] id_list;
		delete[] temp_list;
		delete[] send_list;
	}
	MPI_Finalize();
	return 0;
}