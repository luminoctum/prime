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
public:
	FLOAT *f_value;

protected:
	fftw_plan *plan_forward;
	fftw_plan *plan_backward;

public:
	FFTVariable();

	FFTVariable(const FFTVariable&);

	FFTVariable(const DistGrid&, std::string = "", std::string = "",
			std::string = "", GridSpec = abstract);

	~FFTVariable();

	void make();

	void unmake();

    void fft();

    void ifft();

    std::string value_info() const;

protected:
	void redirect();
};

#endif /* FFTVARIABLE_H_ */
