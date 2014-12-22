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

class DistGrid : public PatchGrid {
	friend std::ostream &operator<<(std::ostream &, const DistGrid &);

public:
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

	virtual ~DistGrid();
};

#endif /* DISTGRID_H_ */
