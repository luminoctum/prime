/*
 * Halo_test.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#include "Halo.h"
#include "PatchVariable.h"
using namespace std;

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
