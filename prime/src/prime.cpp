//============================================================================
// Name        : prime.cpp
// Author      : Cheng Li
// Version     :
// Copyright   : This is a free software
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "mpi.h"
#include <iostream>
#include "Configure.h"
#include "PatchVariable.h"
#include "Halo.h"
#include "DistGrid.h"
using namespace std;


int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

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

	printf("This is %d of %d\n", rank, size);
	cout << grid << endl;
	cout << (Grid)grid << endl;

	MPI_Finalize();
	return 0;
}
