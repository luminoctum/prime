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
class PatchVariable: public PatchGrid, public Variable {
	friend std::ostream &operator<<(std::ostream &, PatchVariable &);

protected:
	std::vector<Variable> tile; /**< This hides the tile in PatchGrid */

	PatchVariable& redirect();

public:
	PatchVariable();

	PatchVariable(const Variable&);

	PatchVariable(const PatchVariable&);

	PatchVariable(const PatchGrid&, std::string = "", std::string = "",
			std::string = "", GridSpec = defaultgrid);

	PatchVariable& operator=(const PatchVariable&);

	PatchVariable& make();

	PatchVariable& unmake();

	PatchVariable& split(int, int);

	inline const Variable& operator[](int p) const{
		return tile[p];
	}

	inline Variable& operator[](int p) {
		return tile[p];
	}

};

/**
 * DistVariable is a distributed variable. Its memory is distributed on different machines
 * communicated by MPI
 */
/*class DistVariable: public PatchVariable {
 public:
 DistVariable();
 DistVariable(const PatchGrid &, std::string,
 std::string, std::string, GridSpec = defaultgrid,
 );
 };*/


#endif /* PATCHVARIABLE_H_ */
