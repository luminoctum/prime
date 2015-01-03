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
	totalprocs,
    stride_x,
    stride_y;

	std::array<int, 2> nprocs,
	isperiodic,
	coordinate;

	std::array<int, 4> neighbour;

	MPI_Comm comm;

public:
	DistGrid();

	DistGrid(const PatchGrid&, const MPI_Comm&);

    int get_num_procs() const {
        return totalprocs;
    };

    int get_id() const {
        return iamnode;
    };

    int get_global_x(int i) const {
        return coordinate[0] * stride_x + i;
    };

    int get_global_y(int j) const {
        return coordinate[1] * stride_y + j;
    };

	std::string head_info() const;

};

#endif /* DISTGRID_H_ */
