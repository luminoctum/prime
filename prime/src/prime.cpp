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
#include "FFTVariableTest.cpp"
using namespace std;


int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

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

    //MultithreadingTest test(comm);
    //test.single_thread();
    //test.multi_thread();
    //test.multi_core();
    //test.multi_thread_core();

	FFTVariableTest test(comm);

	MPI_Finalize();
	return 0;
}
