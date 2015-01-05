/*
 * FFTVariableTest.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#include <mpi.h>
#include <fstream>
#include "FFTVariable.h"
#include "Halo.h"

class FFTVariableTest {
public:
	Configure		config;
	PatchGrid		domain;
	MPI_Comm		comm;
	DistGrid		grid;
	FFTVariable		psi, psi_t;
    Halo            halo;
	std::string		filename;
	std::ofstream	output;

	FFTVariableTest(const MPI_Comm &_comm) :
		config("name.lst"), domain(config), comm(_comm), grid(domain, comm),
		psi(grid, "psi", "streamfunction", "m^2/s", prognostic),
		psi_t(grid, "psi_t", "streamfunction_tendency", "m^2/s")
	{
        if (grid.get_num_procs() == 1)
            filename = "output-single.txt";
        else
            filename = "output-" + std::to_string(grid.get_id()) + ".txt";
        output.open(filename);
		output << "FFT Variable Unit Test Begin" << std::endl;

        psi.make(); psi_t.make();
	}

	~FFTVariableTest(){
		output << "FFT Variable Unit Test End" << std::endl;
		output.close();
	}

    void run(){
        psi.clear_all().set_random_int();

        output << psi << std::endl;

        psi.fft();
        output << (Variable&)psi << std::endl;
        psi.ifft();
        output << (Variable&)psi << std::endl;
    }
};
