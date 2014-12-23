/*
 * patchvariable.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#include "PatchVariable.h"

PatchVariable::PatchVariable(){
}

PatchVariable::PatchVariable(const Variable &var) :
	Grid(var), PatchGrid(var), Variable(var), tile(1, Variable(var)){
	tile[0].unmake();
	redirect();
}

PatchVariable::PatchVariable(const PatchVariable &pv) :
		Grid(pv), PatchGrid(pv), Variable(pv), tile(pv.tile) {
	redirect();
}

PatchVariable::PatchVariable(const PatchGrid& pgrid, std::string _name,
		std::string _long_name, std::string _units, GridSpec _spec) :
		Grid(pgrid), PatchGrid(pgrid), Variable(pgrid, _name, _long_name,
				_units, _spec) {
	for (int p = 0; p < ntiles; p++)
		tile.push_back(
				Variable(PatchGrid::tile[p], name, long_name, units,
						spec | abstract));
	redirect();
}

PatchVariable& PatchVariable::operator=(const PatchVariable &pv) {
	Grid::operator =(pv);
	PatchGrid::operator =(pv);
	Variable::operator =(pv);
	tile = pv.tile;
	redirect();

	return *this;
}

PatchVariable& PatchVariable::make(){
	if (~spec & abstract)
		return *this;
	// make Grid
	Grid::make();

	// make PatchGrid
	for (int p = 0; p < ntiles; p++) {
		PatchGrid::tile.push_back(Grid::sub(ntilex, ntiley, p));
		PatchGrid::tile.back().set_shift_index(shift1d, shift2d, shift3d);
	}
	PatchGrid::redirect();

	// make Variable
	size = nt * nxh * nyh * nzh;
	value = new FLOAT[size];

	// make PatchVariable
	for (int p = 0; p < ntiles; p++) {
		tile.push_back(Variable::sub(ntilex, ntiley, p));
		tile.back().set_shift_index(shift1d, shift2d, shift3d);
		tile.back().set_offset();
	}

	redirect();
	spec &= ~abstract;
	return *this;
}

PatchVariable& PatchVariable::unmake(){
	if (spec & abstract)
		return *this;
	// unmake Grid
	Grid::unmake();

	// unmake PatchGrid
	PatchGrid::tile.clear();

	// unmake Variable
	delete[] value;

	// unmake PatchVariable
	tile.clear();
	redirect();

	return *this;
}

PatchVariable& PatchVariable::redirect() {
	Grid::redirect();
	PatchGrid::redirect();
	Variable::redirect();
	if (spec & abstract)
		return *this;

	for (int p = 0; p < ntiles; p++) {
		int i = p % ntilex;
		int j = p / ntilex;
		tile[p].value = value + i * tile[p].nx + j * tile[p].ny * shift1d;
	}

	return *this;
}

PatchVariable& PatchVariable::split(int tx, int ty) {
	PatchGrid::split(tx, ty);
	tile.clear();
	for (int p = 0; p < ntiles; p++) {
		tile.push_back(Variable::sub(ntilex, ntiley, p));
		tile.back().set_shift_index(shift1d, shift2d, shift3d);
		tile.back().set_offset();
	}
	redirect();

	return *this;
}



