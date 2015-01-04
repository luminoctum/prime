/*
 * Halo.h
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#ifndef HALO_H_
#define HALO_H_

#include "boundary.h"

class Halo {
protected:
	BoundaryFunc fix[4 * N_BOUNDARY_TYPES];

public:
	Halo();

	virtual ~Halo();

	void update(Variable&);
};

#endif /* HALO_H_ */
