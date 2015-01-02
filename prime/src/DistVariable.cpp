/*
 * DistVariable.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: cli
 */

#include "DistVariable.h"
#include <fstream>

DistVariable::DistVariable() : stride_t(0){
	// TODO Auto-generated constructor stub
}

DistVariable::DistVariable(const DistGrid& grid, std::string _name,
		std::string _long_name, std::string _units, GridSpec _spec) :
		Grid(grid), PatchGrid(grid), DistGrid(grid), PatchVariable(grid, _name, _long_name,
				_units, _spec) {
    MPI_Type_vector(ny, nh, nxh, MPI_DOUBLE, &stride_t);
    MPI_Type_commit(&stride_t);
}

void DistVariable::communicate(){
	int tag = 123;
    //double buf[1000];
    //std::ofstream debug;
    //char filename[80];
    //sprintf(filename, "debug-%d.txt", iamnode);
    //debug.open(filename);

    //std::cout << "communicate left" << std::endl;
	MPI_Sendrecv(value + nxh * nh + nh, 1, stride_t, neighbour[0], tag,
            value + nxh * nh + nx + nh, 1, stride_t, neighbour[1], tag, comm, &status);

	//std::cout << "communicate right" << std::endl;
	MPI_Sendrecv(value + nxh * nh + nx, 1, stride_t, neighbour[1], tag, 
            value + nxh * nh, 1, stride_t, neighbour[0], tag, comm, &status);
        
	//std::cout << "communicate bottom" << std::endl;
	MPI_Sendrecv(value + nxh * nh, nxh * nh, MPI_DOUBLE, neighbour[2], tag,
    		value + nxh * (ny + nh), nxh * nh, MPI_DOUBLE, neighbour[3], tag, comm, &status);

	//std::cout << "communicate top" << std::endl;
	MPI_Sendrecv(value + nxh * ny, nxh * nh, MPI_DOUBLE, neighbour[3], tag,
			value, nxh * nh, MPI_DOUBLE, neighbour[2], tag, comm, &status);

    //debug.close();
}

std::string DistVariable::head_info() const{
    std::string result;

	result = "name          : " + name + " (" + long_name + ", " + units
			+ ")\n";
    result += DistGrid::head_info();

    return result;
}
