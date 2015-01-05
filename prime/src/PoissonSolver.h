/*
 * PoissonSolver.h
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#ifndef POISSONSOLVER_H_
#define POISSONSOLVER_H_
#include "FFTVariable.h"

class PoissonSolver {
protected:
	int rank;
	FLOAT *diag, stretch;

public:
	PoissonSolver();

	PoissonSolver(const PoissonSolver&);

	PoissonSolver(int, int);

	virtual ~PoissonSolver();

	PoissonSolver& operator=(const PoissonSolver&);

	void operator()(const FFTVariable&, FFTVariable&);
};

#endif /* POISSONSOLVER_H_ */