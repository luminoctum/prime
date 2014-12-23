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
    	os << "Number of processors: " << grid.nprocs[0] << " x " << grid.nprocs[1]
    			<< " = " << grid.totalprocs << std::endl;
    	os << "I am node : " << grid.iamnode << std::endl;
    	os << "My coordinate is : (" << grid.coordinate[0] << " , "
    			<< grid.coordinate[1] << ")" << std::endl;
    	os << "My neighbour is : " << grid.neighbour[0] << " " << grid.neighbour[1]
    			<< " " << grid.neighbour[2] << " " << grid.neighbour[3] << std::endl;
        os << (PatchGrid)grid << std::endl;
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

};

#endif /* DISTGRID_H_ */
