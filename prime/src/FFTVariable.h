/*
 * FFTVariable.h
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#ifndef FFTVARIABLE_H_
#define FFTVARIABLE_H_
#include "fftw3.h"
#include "stdlib.h"
#include "DistVariable.h"

class FFTVariable: public DistVariable {
protected:
	FLOAT *f_value;
	fftw_plan *plan;

public:
	FFTVariable();

	FFTVariable(const DistGrid&, std::string = "", std::string = "",
			std::string = "", GridSpec = abstract);

	~FFTVariable();

	void make();

	void unmake();

    void fft();

protected:
	void redirect();
};

#endif /* FFTVARIABLE_H_ */
