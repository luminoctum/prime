/*
 * FFTVariable.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#include "FFTVariable.h"

FFTVariable::FFTVariable() : f_value(0), plan(0) {
}

FFTVariable::FFTVariable(const DistGrid& grid, std::string _name,
		std::string _long_name, std::string _units, GridSpec _spec) :
		DistVariable(grid, _name, _long_name, _units, _spec) {
	redirect();
}

FFTVariable::~FFTVariable() {
	unmake();
}

void FFTVariable::make() {
	if (~spec & abstract) return;
	// make Grid
	Grid::make();

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
	PatchVariable::redirect();

	// make FFTVariable
	f_value = new FLOAT[nx * nyh * nzh];
	plan = (fftw_plan*)malloc(nyh * sizeof(fftw_plan));
	for (int j = 0; j < nyh; j++)
		plan[j] = fftw_plan_r2r_1d(nx, value + nh + j * nxh, f_value + j * nx,
				FFTW_R2HC, FFTW_ESTIMATE);
	redirect();
}

void FFTVariable::unmake() {
	// unmake Grid
	Grid::unmake();

	// unmake PatchGrid
	PatchGrid::tile.clear();
	PatchGrid::redirect();

	// unmake Variable
	delete[] value;
	Variable::redirect();

	// unmake PatchVariable
	PatchVariable::tile.clear();
	PatchVariable::redirect();

	// unmake FFTVariable
	delete[] f_value;
	for (int j = 0; j < nyh; j++)
		fftw_destroy_plan(plan[j]);
	free(plan);
	redirect();
}

void FFTVariable::redirect() {
	if (spec & abstract) {
		f_value = 0;
		plan = 0;
	}
}

