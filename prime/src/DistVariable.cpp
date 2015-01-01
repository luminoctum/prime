/*
 * DistVariable.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: cli
 */

#include "DistVariable.h"

DistVariable::DistVariable() {
	// TODO Auto-generated constructor stub
}

DistVariable::DistVariable(const DistGrid& grid, std::string _name,
		std::string _long_name, std::string _units, GridSpec _spec) :
		Grid(grid), PatchGrid(grid), DistGrid(grid), PatchVariable(grid, _name, _long_name,
				_units, _spec) {
    MPI_Type_vector(ny * nh, nh, nxh, MPI_DOUBLE, &coldata);
    MPI_Type_commit(&coldata);
}

void DistVariable::communicate(){
	int tag = 123;

	std::cout << "communicate left" << std::endl;
	MPI_Sendrecv(value + nxh * nh + nh, 1, coldata, neighbour[0], tag,
			value + nxh * nh + nx + nh, 1, coldata, neighbour[1], tag, comm, &status);

	std::cout << "communicate right" << std::endl;
	MPI_Sendrecv(value + nxh * nh + nx, 1, coldata, neighbour[1], tag,
			value + nxh * nh, 1, coldata, neighbour[0], tag, comm, &status);

	std::cout << "communicate bottom" << std::endl;
	MPI_Sendrecv(value + nxh * nh, nxh * nh, MPI_DOUBLE, neighbour[2], tag,
			value + nxh * (ny + nh), nxh * nh, MPI_DOUBLE, neighbour[3], tag, comm, &status);

	std::cout << "communicate top" << std::endl;
	MPI_Sendrecv(value + nxh * ny, nxh * nh, MPI_DOUBLE, neighbour[3], tag,
			value, nxh * nh, MPI_DOUBLE, neighbour[2], tag, comm, &status);

	/*
	std::cout << "communicate left" << std::endl;
	MPI_Sendrecv(value + nxh * nh + nh, 1, coldata, neighbour[0], tag,
			value + nxh * nh, 1, coldata, neighbour[0], tag, comm, &status);

	std::cout << "communicate right" << std::endl;
	MPI_Sendrecv(value + nxh * nh + nx, 1, coldata, neighbour[1], tag,
			value + nxh * nh + nx + nh, 1, coldata, neighbour[1], tag, comm, &status);

	std::cout << "communicate bottom" << std::endl;
	MPI_Sendrecv(value + nxh * nh, nxh * nh, MPI_DOUBLE, neighbour[2], tag,
			value, nxh * nh, MPI_DOUBLE, neighbour[2], tag, comm, &status);

	std::cout << "communicate top" << std::endl;
	MPI_Sendrecv(value + nxh * ny, nxh * nh, MPI_DOUBLE, neighbour[3], tag,
			value, nxh * (ny + nh), MPI_DOUBLE, neighbour[3], tag, comm, &status);
	*/

}

std::string DistVariable::head_info() const{
    std::string result;

	result = "name          : " + name + " (" + long_name + ", " + units
			+ ")\n";
    result += DistGrid::head_info();

    return result;
}
