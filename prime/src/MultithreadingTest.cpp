/*
 * multithreading_test.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: cli
 */

#include <mpi.h>
#include <omp.h>
#include <fstream>
#include "DistVariable.h"

class MultithreadingTest {
public:
    Configure       config;
    PatchGrid       domain;
    MPI_Comm        comm;
    DistGrid        grid;
    DistVariable    psi, psi_t, zeta;
    std::string     filename;
    std::ofstream   output;

    MultithreadingTest(const MPI_Comm &_comm) : 
        config("name.lst"), domain(config), comm(_comm), grid(domain, comm),
        psi(grid, "psi", "streamfunction", "m^2/s", prognostic),
        psi_t(grid, "psi_t", "streamfunction_tendency", "m^2/s"),
        zeta(grid, "zeta", "relative_vorticity", "1/s", prognostic | stagx)
    {
        psi.make(); psi_t.make(); zeta.make();
        if (grid.get_num_procs() == 1)
            filename = "output-single.txt";
        else
            filename = "output-" + std::to_string(grid.get_id()) + ".txt";
        output.open(filename);

        output << "*********************************" << std::endl
            << "* Variable declaration complete *" << std::endl
			<< "*********************************" << std::endl << std::endl;
    }

    ~MultithreadingTest(){
        output.close();
    }

    void laplace(const Variable &src, Variable &dst) {
        for (int i = 0; i < dst.nx; i++)
            for (int j = 0; j < dst.ny; j++) {
                dst(i, j) = (src(i + 1, j) + src(i - 1, j) - 2. * src(i, j)) / (1)
                        + (src(i, j + 1) + src(i, j - 1) - 2. * src(i, j)) / (1);
            }
    }

    void single_thread(){
        psi.clear_all().set_random_int();
        psi_t.clear_all();
        psi.save();
        output << psi << std::endl;
        laplace(psi, psi_t);
        output << psi_t << std::endl;
    }

    void multi_thread(){
        int num_of_threads = 4;
        psi.split(2, 2); psi_t.split(2, 2);
        psi.clear_all().set_random_int();
        psi_t.clear_all();

        omp_set_num_threads(num_of_threads);
        #pragma omp parallel for
        for (int p = 0; p < num_of_threads; p++) {
            output << "Thread : " << omp_get_thread_num() <<  " / " << omp_get_num_threads() << std::endl;
            laplace(psi[p], psi_t[p]);
        }

        output << (Variable&)psi_t << std::endl;
    }

    void multi_core(){
        Variable temp(domain);
        temp.make();
        temp.clear_all().set_random_int();
        psi.clear_all(); psi_t.clear_all();

        for (int j = 0; j < psi.ny; j++)
            for (int i = 0; i < psi.nx; i++)
                psi(i, j) = temp(psi.get_global_x(i), psi.get_global_y(j));
        output << (Variable&)psi << std::endl;
        psi.communicate();
        laplace(psi, psi_t);
        psi_t.communicate();
        output << (Variable&)psi_t << std::endl;
    }

    void multi_thread_core(){
        Variable temp(domain);
        temp.make();
        int num_of_threads = 4;
        //psi.split(2, 2); psi_t.split(2, 2);
        temp.clear_all().set_random_int();
        psi.clear_all(); psi_t.clear_all();
        psi.split(2, 2); psi_t.split(2, 2);

        for (int j = 0; j < psi.ny; j++)
            for (int i = 0; i < psi.nx; i++)
                psi(i, j) = temp(psi.get_global_x(i), psi.get_global_y(j));
        psi.communicate();

        omp_set_num_threads(num_of_threads);
        #pragma omp parallel for
        for (int p = 0; p < num_of_threads; p++) {
            output << "Thread : " << omp_get_thread_num() <<  " / " << omp_get_num_threads() << std::endl;
            laplace(psi[p], psi_t[p]);
        }
        psi_t.communicate();
        output << (Variable&)psi_t << std::endl;
    }

    void run(){
        single_thread();
        Variable single(psi_t);
        single.make();
        single.clear_all();
        single = psi_t;
    }
};
