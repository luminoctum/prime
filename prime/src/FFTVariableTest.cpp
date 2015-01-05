/*
 * FFTVariableTest.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#include <mpi.h>
#include <fstream>
#include "FFTVariable.h"

class FFTVariableTest {
public:
	Configure		config;
	PatchGrid		domain;
	MPI_Comm		comm;
	DistGrid		grid;
	FFTVariable		psi, psi_t;
	std::string		filename;
	std::ofstream	output;

	FFTVariableTest(const MPI_Comm &_comm) :
		config("name.lst"), domain(config), comm(_comm), grid(domain, comm),
		psi(grid, "psi", "streamfunction", "m^2/s", prognostic),
		psi_t(grid, "psi_t", "streamfunction_tendency", "m^2/s")
	{
		output << "FFT Variable Unit Test Begin" << std::endl;
	}

	~FFTVariableTest(){
		output << "FFT Variable Unit Test End" << std::endl;
		output.close();
	}

	void run(){
	}
};
