/*
 * Variable_test.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: cli
 */

#include "PatchVariable.h"
using namespace std;

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
