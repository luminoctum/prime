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
		Grid(grid), ntilex(1), ntiley(1), ntiles(1), tile(1, grid) {
	tile[0].unmake();
	redirect();
}

PatchGrid::PatchGrid(const PatchGrid& pg) :
		Grid(pg), ntilex(pg.ntilex), ntiley(pg.ntiley), ntiles(pg.ntiles), tile(
				pg.tile) {
	redirect();
}

PatchGrid::PatchGrid(const Configure& config, GridSpec gs) :
		Grid(config, gs), ntilex(1), ntiley(1), ntiles(1), tile(1,
				Grid(config, gs | abstract)) {
	redirect();
}

PatchGrid& PatchGrid::operator=(const PatchGrid& pg) {
	Grid::operator=(pg);
	ntilex = pg.ntilex;
	ntiley = pg.ntiley;
	ntiles = pg.ntiles;
	tile = pg.tile;
	redirect();

	return *this;
}

PatchGrid& PatchGrid::make() {
	if (~spec & abstract)
		return *this;
	Grid::make();
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
	Grid::unmake();
	tile.clear();
	redirect();

	return *this;
}

PatchGrid& PatchGrid::split(int tx, int ty) {
	if (spec & abstract){
		std::cerr << "PatchGrid must be concrete before split" << std::endl;
		exit(1);
	}
	// if it is not the case, unmake will not take affect and tile will not be cleared
	unmake();
	ntilex = tx;
	ntiley = ty;
	ntiles = tx * ty;
	make();

	return *this;
}

PatchGrid& PatchGrid::redirect() {
	Grid::redirect();
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

