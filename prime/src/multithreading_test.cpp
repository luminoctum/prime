/*
 * multithreading_test.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: cli
 */

#include "PatchVariable.h"
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


