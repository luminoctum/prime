/*
 * DistVariable.h
 *
 *  Created on: Dec 22, 2014
 *      Author: cli
 */

#ifndef DISTVARIABLE_H_
#define DISTVARIABLE_H_

#include "DistGrid.h"
#include "PatchVariable.h"

/**
 * DistVariable is a distributed variable. Its memory is distributed on different machines
 * communicated by MPI
 */
class DistVariable: public DistGrid, public PatchVariable {
	template<class STREAM>
	friend STREAM &operator<<(STREAM &os, DistVariable &var) {
		os
				<< "-------------------DistVariable Information Begin-----------------"
				<< std::endl;
		os << var.head_info();
		if (~var.spec & abstract) for (int p = 0; p < var.ntiles; p++) {
			os << "Tile # " << p << ": " << std::endl;
			os << var.tile[p];
		}
		os
				<< "--------------------DistVariable Information End------------------"
				<< std::endl;

		return os;
	}
protected:
    MPI_Datatype stride_t;
    MPI_Status status;

public:
	DistVariable();

	DistVariable(const DistGrid&, std::string = "", std::string = "",
			std::string = "", GridSpec = abstract);

	void communicate();

	std::string head_info() const;
};

#endif /* DISTVARIABLE_H_ */
