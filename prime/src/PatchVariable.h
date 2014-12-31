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
	friend STREAM &operator<<(STREAM &os, PatchVariable &var) {
		os
				<< "-------------------PatchVariable Information Begin-----------------"
				<< std::endl;
		os << var.head_info();
		if (~var.spec & abstract) for (int p = 0; p < var.ntiles; p++) {
			os << "Tile # " << p << ":" << std::endl;
			os << var.tile[p].head_info() << var.tile[p].value_info();
		}
		os
				<< "-------------------PatchVariable Information End-----------------"
				<< std::endl;

		return os;
	}

//protected:
public:
	std::vector<Variable> tile; /**< This hides the tile in PatchGrid */

public:
	PatchVariable();

	PatchVariable(const Variable&);

	PatchVariable(const PatchVariable&);

	PatchVariable(const PatchGrid&, std::string = "", std::string = "",
			std::string = "", GridSpec = abstract);

	PatchVariable& operator=(const PatchVariable&);

	PatchVariable& make();

	PatchVariable& unmake();

	inline const Variable& operator[](int p) const {
		return tile[p];
	}

	inline Variable& operator[](int p) {
		return tile[p];
	}

	std::string head_info() const;

protected:
	PatchVariable& redirect();


};

#endif /* PATCHVARIABLE_H_ */
