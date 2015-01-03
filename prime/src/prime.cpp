//============================================================================
// Name        : prime.cpp
// Author      : Cheng Li
// Version     :
// Copyright   : This is a free software
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "mpi.h"
#include <iostream>
#include <fstream>
#include "Configure.h"
#include "Halo.h"
#include "DistVariable.h"
#include "MultithreadingTest.cpp"
using namespace std;


int main(int argc, char *argv[]) {

	MPI_Init(&argc, &argv);

    //char filename[80];
	int size, rank, nprocs[2], isperiodic[2];
	MPI_Comm comm;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1){
        nprocs[0] = 1; nprocs[1] = 1;
        isperiodic[0] = 0; isperiodic[1] = 0;
    } else {
        nprocs[0] = 2; nprocs[1] = size / 2;
        isperiodic[0] = 0; isperiodic[1] = 0;
    }

	MPI_Cart_create(MPI_COMM_WORLD, 2, nprocs, isperiodic, 1, &comm);

    /**
     * Test MPI
	Configure config("name.lst");
	PatchGrid domain(config);
	DistGrid grid(domain, comm);
	DistVariable psi(grid, "psi", "streamfunction", "m^2/s");
	psi.make();
    psi.split(2,2);
	psi.clear_all().set_random_int();
	psi.communicate();

    ofstream output;
    sprintf(filename, "output-%d.txt", rank);
    output.open(filename);

	output << psi << endl;

    Variable &var = psi;
    output << var << endl;
    
    output.close();
     */
    MultithreadingTest test(comm);
    //test.single_thread();
    //test.multi_thread();
    //test.multi_core();
    test.multi_thread_core();

	MPI_Finalize();

	return 0;
}
