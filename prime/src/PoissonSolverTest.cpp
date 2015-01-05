/*
 * PoissonSolverTest.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */
#include <mpi.h>
#include <fstream>
#include "PoissonSolver.h"
#include "Halo.h"

class PoissonSolverTest {
public:
	Configure		config;
	PatchGrid		domain;
	MPI_Comm		comm;
	DistGrid		grid;
	FFTVariable		psi, psi_t, zeta;
	std::string		filename;
    Halo            halo;
	std::ofstream	output;
    PoissonSolver   poisson_solver;

	PoissonSolverTest(const MPI_Comm &_comm) :
		config("name.lst"), domain(config), comm(_comm), grid(domain, comm),
		psi(grid, "psi", "streamfunction", "m^2/s", prognostic),
		psi_t(grid, "psi_t", "streamfunction_tendency", "m^2/s"),
        zeta(grid, "zeta", "vorticity", "1/s"),
        poisson_solver(grid.nx, grid.ny, grid.nh)
	{
        if (grid.get_num_procs() == 1)
            filename = "output-single.txt";
        else
            filename = "output-" + std::to_string(grid.get_id()) + ".txt";
        output.open(filename);
		output << "Poisson Solver Unit Test Begin" << std::endl;

        psi.make(); psi_t.make(); zeta.make();
	}

	~PoissonSolverTest(){
		output << "Poisson Solver Unit Test End" << std::endl;
		output.close();
	}

    void laplace(const Variable &src, Variable &dst) {
        for (int i = 0; i < dst.nx; i++)
            for (int j = 0; j < dst.ny; j++) {
                dst(i, j) = (src(i + 1, j) + src(i - 1, j) - 2. * src(i, j)) / (1)
                        + (src(i, j + 1) + src(i, j - 1) - 2. * src(i, j)) / (1);
            }
    }

    void run() {
        psi.clear_all().set_random_int();
        psi_t.clear_all();
        zeta.clear_all();
        halo.update(psi);

        output << psi << std::endl;
        laplace(psi, psi_t);
        psi_t.fft();
        output << (Variable&)psi_t << std::endl;
        poisson_solver(psi_t, zeta);
        output << (Variable&)zeta << std::endl;
    }
};

