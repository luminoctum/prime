/*
 * PoissonSolver.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#include "PoissonSolver.h"

PoissonSolver::PoissonSolver() : rank(0), diag(0), stretch(0) {
}

PoissonSolver::PoissonSolver(const PoissonSolver& solver) :
	rank(solver.rank), stretch(solver.stretch){
	diag = new FLOAT[rank];
}

PoissonSolver::PoissonSolver(int _rank, int _stretch) :
		rank(_rank), stretch(_stretch) {
	diag = new FLOAT[rank];
}

PoissonSolver::~PoissonSolver() {
	delete[] diag;
}

PoissonSolver& PoissonSolver::operator=(const PoissonSolver& solver) {
	rank = solver.rank;
	diag = solver.diag;
	stretch = solver.stretch;
	delete[] diag;
	diag = new FLOAT[rank];

	return *this;
}

void PoissonSolver::operator()(const FFTVariable& var, FFTVariable& result) {
	FLOAT m;
	for (int k = 0; k < var.nx; k++){
		for (int i = 0; i < rank; i++)
			diag[i] = 2. * cos(2. * PI * i / rank) - 4. - stretch;
		for (int i = 1; i < rank; i++) {
			m = 1. / diag[i - 1];
			diag[i] = diag[i] - m;
			var.fvalue[i] = var.fvalue[i] - m * var.fvalue[i - 1];
		};
		result.fvalue[rank - 1] = var.fvalue[rank - 1] / diag[rank - 1];
		for (long i = rank - 2; i >= 0; i--)
			result.fvalue[i] = (var.fvalue[i] - var.fvalue[i + 1]) / diag[i];
	}
	result.ifft();
}
