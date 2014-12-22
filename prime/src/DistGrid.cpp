/*
 * DistGrid.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: cli
 */

#include "DistGrid.h"

std::ostream &operator<<(std::ostream &os, const DistGrid &grid) {
	os << "-------------------DistGrid Information Begin-----------------"
			<< std::endl;
	os << "Number of processors: " << grid.nprocs[0] << " x " << grid.nprocs[1]
			<< " = " << grid.totalprocs;
	os << std::endl;
	os << "I am node : " << grid.iamnode;
	os << std::endl;
	os << "My coordinate is : (" << grid.coordinate[0] << " , "
			<< grid.coordinate[1] << ")";
	os << std::endl;
	os << "My neighbour is : " << grid.neighbour[0] << " " << grid.neighbour[1]
			<< " " << grid.neighbour[2] << " " << grid.neighbour[3];
	os << std::endl;
	os << "-------------------DistGrid Information End-----------------"
			<< std::endl;
	return os;
}

DistGrid::DistGrid() :
		dimension(2), iamnode(0), totalprocs(0), nprocs { { 0, 0 } }, isperiodic {
				{ 0, 0 } }, coordinate { { 0, 0 } }, neighbour { { 0, 0, 0, 0 } }, comm(
				0) {
}

DistGrid::DistGrid(const PatchGrid &grid, const MPI_Comm& _comm) :
		dimension(2), comm(_comm) {
	MPI_Comm_size(comm, &totalprocs);
	MPI_Comm_rank(comm, &iamnode);

	int dims[dimension], periods[dimension], coords[dimension];
	MPI_Cart_get(comm, dimension, dims, periods, coords);
	for (int i = 0; i < dimension; i++) {
		nprocs[i] = dims[i];
		isperiodic[i] = periods[i];
		coordinate[i] = coords[i];
	}
	int left, right, bottom, top;
	MPI_Cart_shift(comm, 0, 1, &left, &right);
	MPI_Cart_shift(comm, 1, 1, &bottom, &top);
	neighbour[0] = left;
	neighbour[1] = right;
	neighbour[2] = bottom;
	neighbour[3] = top;

	PatchGrid::operator=(
			grid.sub(dims[0], dims[1], coords[0] + coords[1] * dims[0]));

}

DistGrid::~DistGrid() {
	// TODO Auto-generated destructor stub
}

