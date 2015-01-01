/*
 * DistGrid.h
 *
 *  Created on: Dec 21, 2014
 *      Author: cli
 */

#ifndef DISTGRID_H_
#define DISTGRID_H_

#include "mpi.h"
#include <array>
#include "PatchGrid.h"

/**
 * DistGrid distribute grid on different machines.
 */
class DistGrid : public virtual PatchGrid {
    template<class STREAM>
	friend STREAM &operator<<(STREAM &os, const DistGrid &grid){
    	os << "--------------------DistGrid Information Begin-------------------"
    			<< std::endl;
    	os << grid.head_info();
    	os << "--------------------DistGrid Information End---------------------"
    			<< std::endl;
    	return os;
    }

protected:
	int dimension,
	iamnode,
	totalprocs;

	std::array<int, 2> nprocs,
	isperiodic,
	coordinate;

	std::array<int, 4> neighbour;

	MPI_Comm comm;

public:
	DistGrid();

	DistGrid(const PatchGrid&, const MPI_Comm&);

	std::string head_info() const;

};

#endif /* DISTGRID_H_ */
