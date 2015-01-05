/*
 * PoissonSolver.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#include "PoissonSolver.h"

PoissonSolver::PoissonSolver() : mode(0), rank(0), halo(0), diag(0), stretch(0) {
}

PoissonSolver::PoissonSolver(const PoissonSolver& solver) :
	mode(solver.mode), rank(solver.rank), halo(solver.halo), stretch(solver.stretch){
	diag = new FLOAT[rank];
}

PoissonSolver::PoissonSolver(int _mode, int _rank, int _halo, int _stretch) :
		mode(_mode), rank(_rank), halo(_halo), stretch(_stretch) {
	diag = new FLOAT[rank];
}

PoissonSolver::~PoissonSolver() {
	delete[] diag;
}

PoissonSolver& PoissonSolver::operator=(const PoissonSolver& solver) {
    mode = solver.mode;
	rank = solver.rank;
    halo = solver.halo;
	stretch = solver.stretch;
	diag = solver.diag;
	delete[] diag;
	diag = new FLOAT[rank];

	return *this;
}

void PoissonSolver::operator()(const FFTVariable& var, FFTVariable& result) {
	FLOAT m;
	for (int k = 0; k < mode; k++){
		for (int i = 0; i < rank; i++)
			diag[i] = 2. * cos(2. * PI * k / mode) - 4. - stretch;
		for (int i = 1; i < rank; i++) {
			m = 1. / diag[i - 1];
			diag[i] -= m;
			var.fvalue[k + (halo + i) * mode] -= m * var.fvalue[k + (halo + i - 1) * mode];
		};
		result.fvalue[k + (halo + rank - 1) * mode] = var.fvalue[k + (halo + rank - 1) * mode] / diag[rank - 1];
		for (int i = rank - 2; i >= 0; i--)
			result.fvalue[k + (halo + i) * mode] = (var.fvalue[k + (halo + i) * mode] - result.fvalue[k + (halo + i + 1) * mode]) / diag[i];
	}
    /** 
     * debug 
    for (int i = 1; i < rank - 1; i++) {
        for (int k = 0; k < mode; k++) {
            m = result.fvalue[k + (halo + i - 1) * mode] 
                + (2. * cos(2. * PI * k / mode) - 4. - stretch) * result.fvalue[k + (halo + i) * mode] 
                + result.fvalue[k + (halo + i + 1) * mode];
            std::cout << m << " ";
        }
        std::cout << std::endl;
    }*/
	result.ifft();
}
