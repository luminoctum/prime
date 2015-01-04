/*
 * patchvariable.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#include "PatchVariable.h"

PatchVariable::PatchVariable() {
}

PatchVariable::PatchVariable(const Variable &var) :
		Grid(var), PatchGrid(var), Variable(var) {
}

PatchVariable::PatchVariable(const PatchVariable &pv) :
		Grid(pv), PatchGrid(pv), Variable(pv) {
}

PatchVariable::PatchVariable(const PatchGrid& pgrid, std::string _name,
		std::string _long_name, std::string _units, GridSpec _spec) :
		Grid(pgrid), PatchGrid(pgrid), Variable(pgrid, _name, _long_name,
				_units, _spec) {
}

PatchVariable& PatchVariable::operator=(const PatchVariable &pv) {
	Grid::operator =(pv);
	PatchGrid::operator =(pv);
	Variable::operator =(pv);
	tile = pv.tile;
	if (~spec & abstract) {
		unmake();
		make();
	} else {
		redirect();
	}

	return *this;
}

void PatchVariable::make() {
	if (~spec & abstract) return;
	// make Grid
	Grid::make();

	// make PatchGrid
	for (int p = 0; p < ntiles; p++)
		PatchGrid::tile.push_back(Grid::sub(ntilex, ntiley, p));
	PatchGrid::redirect();

	// make Variable
	size = nt * nxh * nyh * nzh;
	value = new FLOAT[size];
	Variable::redirect();

	// make PatchVariable
	for (int p = 0; p < ntiles; p++) {
		tile.push_back(Variable::sub(ntilex, ntiley, p));
		tile[p].set_shift_index(shift1d, shift2d, shift3d);
		tile[p].slice();
	}

	redirect();
}

void PatchVariable::unmake() {
	//std::cout << "PatchVariable unmake" << std::endl;
	if (spec & abstract) return;
	// unmake Grid
	Grid::unmake();

	// unmake PatchGrid
	PatchGrid::tile.clear();
	PatchGrid::redirect();

	// unmake Variable
	delete[] value;
	Variable::redirect();

	// unmake PatchVariable
	tile.clear();
	redirect();
}

void PatchVariable::split(int tx, int ty) {
    ntilex = tx;
    ntiley = ty;
    ntiles = tx * ty;
    if (~spec & abstract) {
        // redirect PatchGrid
        PatchGrid::tile.clear();
        for (int p = 0; p < ntiles; p++)
            PatchGrid::tile.push_back(Grid::sub(ntilex, ntiley, p));
        PatchGrid::redirect();

        // redirect PatchVariable
        tile.clear();
        for (int p = 0; p < ntiles; p++) {
            tile.push_back(Variable::sub(ntilex, ntiley, p));
            tile[p].set_shift_index(shift1d, shift2d, shift3d);
            tile[p].slice();
        }
        redirect();
    } else {
        redirect();
    }
}

void PatchVariable::redirect() {
	if (spec & abstract) return;
	for (int p = 0; p < ntiles; p++) {
		int i = p % ntilex;
		int j = p / ntilex;
		tile[p].value = value + i * tile[p].nx + j * tile[p].ny * shift1d;
	}
}

std::string PatchVariable::head_info() const {
	std::string result;
	char buf[100];

	result = "name          : " + name + " (" + long_name + ", " + units
			+ ")\n";
	sprintf(buf, "%-14s: %d x %d = %d\n", "tiles", ntilex, ntiley, ntiles);
	result += buf;
	result += Grid::head_info();

	return result;
}
