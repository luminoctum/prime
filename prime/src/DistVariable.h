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
	friend STREAM &operator<<(STREAM &os, DistVariable &var){
    	os << "-------------------DistVariable Information Begin-----------------"
    			<< std::endl;
    	for (int p = 0; p < var.ntiles; p++) {
    		os << "Tile # " << p << ": " << std::endl << var.tile[p] << std::endl;
    	}
    	os << "-------------------DistVariable Information End-----------------"
    			<< std::endl;

    	return os;
    }

public:
	DistVariable();

	DistVariable(const DistGrid&, std::string = "", std::string = "",
			std::string = "", GridSpec = defaultgrid);

	DistVariable& communicate();
};

#endif /* DISTVARIABLE_H_ */
