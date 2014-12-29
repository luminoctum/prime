/*
 * Grid_test.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: cli
 */

#include "PatchGrid.h"
using namespace std;

void test_grid() {
	Configure config("name.lst");
	Grid grid(config);
	cout << grid << endl;
	cout << "aa" << grid.sub(1,1,0) << endl;
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
	cout << grid << endl;
	grid.split(2, 2);
	cout << grid << endl;

	PatchGrid grid2;
	cout << grid2 << endl;

	grid2 = grid;
	cout << grid2 << endl;
	grid2.make();
	cout << grid2 << endl;
	for (int i = 0; i < grid2[0].nx; i++)
		cout << grid2[0].lon[i] << " ";
	cout << endl;

}

void test_subpatchgrid() {
	Configure config("name.lst");
	PatchGrid grid(config);
	//grid = grid.sub(2, 2, 1).make();
	//cout << (Grid) grid << endl;
	//cout << grid << endl;
	cout << grid.sub(2, 2, 1) << endl;
}
