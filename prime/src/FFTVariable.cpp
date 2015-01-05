/*
 * FFTVariable.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#include "FFTVariable.h"

FFTVariable::FFTVariable() : fvalue(0), plan_forward(0), plan_backward(0) {
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
	//fvalue = new FLOAT[nx * nyh * nzh];
	fvalue = new FLOAT[nxh * nyh * nzh];
    plan_forward = new fftw_plan[nyh];
    plan_backward = new fftw_plan[nyh];
	for (int j = 0; j < nyh; j++) {
		//plan_forward[j] = fftw_plan_r2r_1d(nx, value + nh + j * nxh, fvalue + j * nx,
		plan_forward[j] = fftw_plan_r2r_1d(nxh, value + j * nxh, fvalue + j * nxh,
				FFTW_R2HC, FFTW_ESTIMATE);
		//plan_backward[j] = fftw_plan_r2r_1d(nx, fvalue + j * nx, value + nh + j * nxh,
		plan_backward[j] = fftw_plan_r2r_1d(nxh, fvalue + j * nxh, value + j * nxh,
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
	delete[] fvalue;

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
		fvalue = 0;
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
    for (int j = -nh; j < ny + nh; j++)
        //for (int i = 0; i < nx; i++)
        //    (*this)(i, j) = (*this)(i, j) / nx;
        for (int i = -nh; i < nx + nh; i++)
            (*this)(i, j) = (*this)(i, j) / nxh;
}

std::string FFTVariable::value_info() const {
    std::string result;
    char buf[100];

    result = "";
    result += Variable::value_info();
    sprintf(buf, "\nFFT :  |");
    result += buf;
    //for (int i = 0; i < nx; i++) {
    //    sprintf(buf, "%8d", i);
    for (int i = 0; i < nxh; i++) {
        sprintf(buf, "%8d", i - nh);
        result += buf;
    }
    result += "\n";
    //for (int i = -1; i < nx; i++)
    for (int i = -1; i < nxh; i++)
        result += "--------";
    result += "\n";
    for (int j = 0; j < nyh; j++) {
        sprintf(buf, "%7d|", j - nh);
        result += buf;
        //for (int i = 0; i < nx; i++) {
        for (int i = 0; i < nxh; i++) {
            std::stringstream ss;
            //ss << std::setw(8) << fvalue[i + j * nx];
            ss << std::setw(8) << fvalue[i + j * nxh];
            result += ss.str();
        }
        result += "\n";
    }

    return result;
}
