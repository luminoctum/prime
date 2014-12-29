/*
 * patchvariable.h
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#ifndef PATCHVARIABLE_H_
#define PATCHVARIABLE_H_

#include "PatchGrid.h"
#include "Variable.h"

/**
 * PatchVariable allocated array on PatchGrid, designed for multithreading.
 * PatchVariable has the normal "value" member. It has has a vector of
 * Variable named "subvar". subvar a Variable, therefore it supports all
 * functions defined in Variable. But it is strictly not a variable in the
 * sense that each "tile" is not strictly a Grid. nxy, nyz, nxyz do not
 * represent the normal meanings.
 */
class PatchVariable: public virtual PatchGrid, public Variable {
    template<class STREAM>
	friend STREAM &operator<<(STREAM &os, PatchVariable &var){
    	os << "-------------------PatchVariable Information Begin-----------------"
    			<< std::endl;
    	os << "name: " << var.name << std::endl << "long name : " << var.long_name
    			<< std::endl << "units : " << var.units << " offset = "
    			<< var.offset << std::endl;
    	os << "longitude: [" << var.lonbot << ":" << var.dlon << ":"
    			<< var.lontop << "], nx = " << var.nx << ", nxh = " << var.nxh
    			<< ", dx = " << var.dx / 1.E3 << " km" << std::endl;
    	os << "latitude: [" << var.latbot << ":" << var.dlat << ":" << var.lattop
    			<< "], ny = " << var.ny << ", nyh = " << var.nyh << ", dy = "
    			<< var.dy / 1.E3 << " km" << std::endl;
    	os << "vertical: Log[" << var.pbot << ":" << exp(var.dlnp) << ":"
    			<< var.ptop << "], nz = " << var.nz << ", nzh = " << var.nzh
    			<< ", dz = " << var.dz / 1.E3 << " km" << std::endl;
    	os << "number of time steps stored: " << var.nt << std::endl;
    	os << var.spec << std::endl;
    	os << "Boundary condition: ";
    	for (int i = 0; i < 6; i++)
    		os << var.boundary[i] << " ";
    	os << std::endl;
    	if (~ var.spec & abstract) {
    		for (int p = 0; p < var.ntiles; p++) {
    			os << "Tile # " << p << ": " << std::endl;
    			os << var.tile[p] << std::endl;
    		}
    	}
    	os << "-------------------PatchVariable Information End-----------------"
    			<< std::endl;

    	return os;
    }

protected:
	std::vector<Variable> tile; /**< This hides the tile in PatchGrid */

	PatchVariable& redirect();

public:
	PatchVariable();

	PatchVariable(const Variable&);

	PatchVariable(const PatchVariable&);

	PatchVariable(const PatchGrid&, std::string = "", std::string = "",
			std::string = "", GridSpec = abstract);

	PatchVariable& operator=(const PatchVariable&);

	PatchVariable& make();

	PatchVariable& unmake();

	inline const Variable& operator[](int p) const{
		return tile[p];
	}

	inline Variable& operator[](int p) {
		return tile[p];
	}

};


#endif /* PATCHVARIABLE_H_ */
