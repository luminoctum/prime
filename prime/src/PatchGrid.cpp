/*
 * patchgrid.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#include "PatchGrid.h"

PatchGrid::PatchGrid() :
		ntilex(0), ntiley(0), ntiles(0) {
}

PatchGrid::PatchGrid(const Grid& grid) :
		Grid(grid), ntilex(1), ntiley(1), ntiles(1) {
}

PatchGrid::PatchGrid(const PatchGrid& pg) :
		Grid(pg), ntilex(pg.ntilex), ntiley(pg.ntiley), ntiles(pg.ntiles) {
}

PatchGrid::PatchGrid(const Configure& config, GridSpec gs) :
		Grid(config, gs), ntilex(1), ntiley(1), ntiles(1) {
}

PatchGrid& PatchGrid::operator=(const PatchGrid& grid) {
	Grid::operator=(grid);
	ntilex = grid.ntilex;
	ntiley = grid.ntiley;
	ntiles = grid.ntiles;

    if (~spec & abstract){
        unmake();
        make();
    } else {
        redirect();
    }

	return *this;
}

PatchGrid& PatchGrid::make() {
	if (~spec & abstract)
		return *this;
    // make Grid
	Grid::make();

    // make PatchGrid
	for (int p = 0; p < ntiles; p++) {
		tile.push_back(Grid::sub(ntilex, ntiley, p));
		tile.back().set_shift_index(shift1d, shift2d, shift3d);
	}
	redirect();

	return *this;
}

PatchGrid& PatchGrid::unmake() {
	if (spec & abstract)
		return *this;
    // unmake Grid
	Grid::unmake();

    // unmake PatchGrid
	tile.clear();
    redirect();

	return *this;
}

PatchGrid& PatchGrid::split(int tx, int ty) {
	ntilex = tx;
	ntiley = ty;
	ntiles = tx * ty;
    if (~spec & abstract){
        unmake();
        make();
    } else {
        redirect();
    }

	return *this;
}

PatchGrid& PatchGrid::redirect() {
	if (spec & abstract)
		return *this;

	for (int j = 0; j < ntiley; j++)
		for (int i = 0; i < ntilex; i++) {
			Grid &atile = tile[i + j * ntilex];
			atile.lon = lon + i * atile.nx;
			atile.rlon = lon + i * atile.nx;
			atile.mlon = lon + i * atile.nx;
			atile.lat = lon + j * atile.ny;
			atile.rlat = lon + j * atile.ny;
			atile.mlat = lon + j * atile.ny;
		}

	return *this;
}

std::string PatchGrid::head_info() const{
	std::string result;
	char buf[100];

	sprintf(buf, "%-14s: %d x %d = %d\n", "tiles", ntilex, ntiley, ntiles);
	result = buf;
	result += Grid::head_info();

	return result;
}

