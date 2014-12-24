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
using namespace std;


int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

    char filename[80];
	int size, rank, nprocs[2], isperiodic[2];
	MPI_Comm comm;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	nprocs[0] = 4; nprocs[1] = 2;
	isperiodic[0] = 1; isperiodic[1] = 0;

	MPI_Cart_create(MPI_COMM_WORLD, 2, nprocs, isperiodic, 1, &comm);

	Configure config("name.lst");
	PatchGrid domain(config);
	DistGrid grid(domain, comm);
	grid.split(2,2);
	PatchVariable psi(grid, "psi", "streamfunction", "m^2/s");
	psi.clear_all().set_random_int();

    ofstream output;
    sprintf(filename, "output-%d.txt", rank);
    output.open(filename);

	output << (Variable)psi << endl;
    
    output.close();

	MPI_Finalize();
	return 0;
}
