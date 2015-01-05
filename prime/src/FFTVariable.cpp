/*
 * FFTVariable.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#include "FFTVariable.h"

FFTVariable::FFTVariable() : f_value(0), plan_forward(0), plan_backward(0) {
}

FFTVariable::FFTVariable(const DistGrid& grid, std::string _name,
		std::string _long_name, std::string _units, GridSpec _spec) :
		Grid(grid), PatchGrid(grid), DistVariable(grid, _name, _long_name, _units, _spec) {
	redirect();
}

FFTVariable::FFTVariable(const FFTVariable& var) :
	Grid(var), PatchGrid(var), DistVariable(var)
{
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
    plan_forward = new fftw_plan[nyh];
    plan_backward = new fftw_plan[nyh];
	for (int j = 0; j < nyh; j++) {
		plan_forward[j] = fftw_plan_r2r_1d(nx, value + nh + j * nxh, f_value + j * nx,
				FFTW_R2HC, FFTW_ESTIMATE);
		plan_backward[j] = fftw_plan_r2r_1d(nx, f_value + j * nx, value + nh + j * nxh,
				FFTW_HC2R, FFTW_ESTIMATE);
	}
	redirect();
}

void FFTVariable::unmake() {
    if (spec & abstract) return;

	//std::cout << "unmake FFTVariable" << std::endl;
	for (int j = 0; j < nyh; j++) {
		fftw_destroy_plan(plan_backward[j]);
		fftw_destroy_plan(plan_forward[j]);
	}
	delete[] plan_backward;
    delete[] plan_forward;
	delete[] f_value;

	//std::cout << "unmake PatchVariable" << std::endl;
	PatchVariable::tile.clear();

	//std::cout << "unmake Variable" << std::endl;
	delete[] value;

	//std::cout << "unmake PatchGrid" << std::endl;
	PatchGrid::tile.clear();

	//std::cout << "unmake Grid" << std::endl;
	Grid::unmake();

	PatchGrid::redirect();
	Variable::redirect();
	PatchVariable::redirect();
    redirect();
}

void FFTVariable::redirect() {
	if (spec & abstract) {
		f_value = 0;
		plan_forward = 0;
		plan_backward = 0;
	}
}

void FFTVariable::fft() {
    for (int j = 0; j < nyh; j++)
        fftw_execute(plan_forward[j]);
}

void FFTVariable::ifft() {
	for (int j = 0; j < nyh; j++)
		fftw_execute(plan_backward[j]);
}

