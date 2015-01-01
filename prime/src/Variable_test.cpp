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
	zeta.make();
	cout << zeta.clear_all() << endl;
	psi.make();
	psi.clear_all().set_random_int().save();
	cout << psi << endl;
	psi.slice(1).set_random_int(-5, 5);
	zeta = psi.slice(0);
	cout << psi << endl;
	cout << zeta << endl;

}

void test_patchvariable() {
	Configure config("name.lst");
	PatchGrid grid(config);
	PatchVariable psi(grid, "psi", "stream_function", "m^2/s", prognostic);
	psi.make();
	psi.clear_all().set_random_int();

	cout << psi << endl;
	cout << static_cast<Variable>(psi) << endl;
	psi.split(2,2);
	psi.clear_all().set_random_int();
	psi[0].save();

	cout << psi << endl;


}

void test_subpatchvariable() {
	Configure config("name.lst");
	PatchGrid grid(config);
	PatchVariable psi(grid, "psi", "stream_function", "m/s", prognostic);
	psi.make();
	psi.clear_all().set_random_int();
	// Variable must make() before split
	psi.split(1, 2);
	psi[0].save();
	cout << (Variable) psi << endl;

	cout << psi.sub(2, 2, 1) << endl;
}
