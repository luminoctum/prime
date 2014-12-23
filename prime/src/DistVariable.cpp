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
		PatchGrid(grid), DistGrid(grid), PatchVariable(grid, _name, _long_name,
				_units, _spec) {
}

DistVariable& DistVariable::communicate(){

	return *this;
}
