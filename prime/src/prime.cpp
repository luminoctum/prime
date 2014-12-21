//============================================================================
// Name        : prime.cpp
// Author      : Cheng Li
// Version     :
// Copyright   : This is a free software
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Configure.h"
#include "PatchVariable.h"
#include "Halo.h"
using namespace std;

void laplace(const Variable &src, Variable &dst) {
	cout << "Inside laplace function." << endl;
	for (int i = 0; i < dst.nx; i++)
		for (int j = 0; j < dst.ny; j++) {
			dst(i, j) = (src(i + 1, j) + src(i - 1, j) - 2. * src(i, j)) / (1)
					+ (src(i, j + 1) + src(i, j - 1) - 2. * src(i, j)) / (1);
		}
	cout << "Laplace function complete." << endl;
}

void test_configure() {
	Configure config("name.lst");
	cout << config << endl;
}

void test_grid() {
	Configure config("name.lst");
	Grid grid(config);
	cout << grid << endl;
	Grid subgrid;
	cout << subgrid << endl;
	for (int i = 0; i < 4; i++) {
		subgrid = grid.sub(2, 2, i);
		cout << "subgrid # " << i << endl << subgrid << endl;
		//cout << "subgrid # " << i << endl << grid.sub(2,2,i) << endl;
	}
	cout << grid.sub(1, 1, 0) << endl;

}

void test_patchgrid() {
	Configure config("name.lst");
	PatchGrid grid(config);
	grid.split(2, 2);
	cout << grid << endl;

	PatchGrid grid2;
	cout << grid2 << endl;

	grid2 = grid;
	cout << grid2 << endl;
	cout << (Grid) grid2 << endl;
	for (int i = 0; i < grid2[0].nx; i++)
		cout << grid2[0].lon[i] << " ";
	cout << endl;

}

void test_variable() {
	Configure config("name.lst");
	Grid grid(config);
	Variable psi(grid, "psi", "stream_function", "m^2/s", prognostic);
	Variable zeta(grid, "zeta", "vorticity", "1/s");
	psi.clear_all().set_random_int().save();
	cout << psi << endl;
	psi.set_offset(1).set_random_int(-5, 5);
	zeta.clear_all();
	zeta = psi.set_offset(1);
	cout << psi << endl;
	cout << zeta << endl;

	laplace(psi, zeta);
	cout << zeta << endl;
}

void test_patchvariable() {
	Configure config("name.lst");
	PatchGrid grid(config);
	grid.split(2, 2);
	//cout << grid << endl;
	PatchVariable psi(grid, "psi", "stream_function", "m^2/s", prognostic);
	PatchVariable zeta(grid, "zeta", "vorticity", "1/s");
	psi.clear_all().set_random_int();
	cout << (Variable) psi << endl;
	cout << psi << endl;
	psi.split(1, 4);
	cout << psi << endl;

}

void test_multithreading() {
	int num_of_threads = 4;
	Configure config("name.lst");
	PatchGrid pgrid(config);
	pgrid.split(2, 2);

	PatchVariable psi(pgrid, "psi", "streamfunction", "m^2/s", prognostic);
	PatchVariable psi_t(pgrid, "psi_t", "streamfunction_tendency", "m^2/s");
	PatchVariable zeta(pgrid, "zeta", "relative_vorticity", "1/s",
			prognostic | stagx);

	cout << "*********************************" << endl
			<< "* Variable declaration complete *" << endl
			<< "*********************************" << endl << endl;

	psi.clear_all().set_random_int();
	psi_t.clear_all();
	zeta.clear_all();

	/* multi-threaded version
	 * This does not work for windows
	 */
	//omp_set_num_threads(num_of_threads);
	//#pragma omp parallel for
	for (int p = 0; p < num_of_threads; p++) {
		//printf("Thread %2u/%2u\n", omp_get_thread_num(), omp_get_num_threads());
		laplace(psi[p], psi_t[p]);
	}
	cout << (Variable) psi << endl;
	cout << (Variable) psi_t << endl;
}

void test_subpatchgrid() {
	Configure config("name.lst");
	PatchGrid grid(config);
	//grid = grid.sub(2, 2, 1).make();
	//cout << (Grid) grid << endl;
	//cout << grid << endl;
	cout << grid.sub(2, 2, 1) << endl;
}

void test_subpatchvariable() {
	Configure config("name.lst");
	PatchGrid grid(config);
	PatchVariable psi(grid, "psi", "stream_function", "m/s", prognostic);
	psi.clear_all().set_random_int();
	psi.split(1, 2);
	psi[0].save();
	cout << (Variable) psi << endl;

	cout << psi.sub(2, 2, 1) << endl;
}

void test_boundary() {
	Halo halo;
	Configure config("name.lst");
	Grid grid(config);
	Variable psi(grid, "psi", "stream_function", "m^2/s");
	psi.clear_all().set_random_int();
	cout << psi << endl;
	cout << "Halo update begin" << endl;
	halo.update(psi);
	cout << "Halo update end" << endl;
	cout << psi << endl;
	cout << (Grid)psi << endl;

	PatchGrid grid2(config);
	grid2.split(2,2);
	PatchVariable psi2(grid2, "psi", "stream_function", "m^2/s");
	psi2.clear_all().set_random_int();
	cout << "Halo update begin" << endl;
	halo.update(psi2);
	cout << "Halo update end" << endl;
	cout << psi2 << endl;
}

int main() {
	//test_configure();
	//test_grid();
	//test_patchgrid();
	//test_variable();
	//test_patchvariable();
	test_multithreading();
	//test_subpatchgrid();
	//test_subpatchvariable();
	//test_boundary();

	/*
	 Configure config("name.lst");
	 PatchGrid grid(config);
	 grid.split(2,2);
	 PatchVariable psi(grid, "psi", "psi", "1");
	 PatchVariable zeta(grid, "zeta", "zeta", "1");
	 psi.clear_all().set_random_int();
	 zeta.clear_all();

	 cout << "*** Declaration Complete. ***" << endl;

	 laplace(psi, zeta);

	 cout << zeta << endl;

	 cout << "*** Calculation Complete. ***" << endl;
	 */


	return 0;
}
