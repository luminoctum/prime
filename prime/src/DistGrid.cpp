/*
 * DistGrid.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: cli
 */

#include "DistGrid.h"

DistGrid::DistGrid() :
		dimension(2), iamnode(0), totalprocs(0), nprocs { { 0, 0 } }, isperiodic {
				{ 0, 0 } }, coordinate { { 0, 0 } }, neighbour { { 0, 0, 0, 0 } }, comm(
				0) {
}

DistGrid::DistGrid(const PatchGrid &grid, const MPI_Comm& _comm) :
		dimension(2), comm(_comm) {
	MPI_Comm_size(comm, &totalprocs);
	MPI_Comm_rank(comm, &iamnode);
	MPI_Cart_get(comm, dimension, &nprocs[0], &isperiodic[0], &coordinate[0]);

	int left, right, bottom, top;
	MPI_Cart_shift(comm, 0, 1, &left, &right);
	MPI_Cart_shift(comm, 1, 1, &bottom, &top);
	neighbour[0] = left;
	neighbour[1] = right;
	neighbour[2] = bottom;
	neighbour[3] = top;

	PatchGrid::operator=(
			grid.sub(nprocs[0], nprocs[1], coordinate[0] + coordinate[1] * nprocs[0]));
}


