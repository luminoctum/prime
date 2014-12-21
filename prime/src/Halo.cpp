/*
 * Halo.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#include "Halo.h"

Halo::Halo() {
	fix[0] = none_0;
	fix[1] = none_0;
	fix[2] = none_0;
	fix[3] = none_0;
	fix[4] = periodic_0;
	fix[5] = periodic_1;
	fix[6] = periodic_2;
	fix[7] = periodic_3;
	fix[8] = zero_gradient_0;
	fix[9] = zero_gradient_1;
	fix[10] = zero_gradient_2;
	fix[11] = zero_gradient_3;
	fix[12] = fixed_gradient_0;
	fix[13] = fixed_gradient_1;
	fix[14] = fixed_gradient_2;
	fix[15] = fixed_gradient_3;
	fix[16] = symmetric_0;
	fix[17] = symmetric_1;
	fix[18] = symmetric_2;
	fix[19] = symmetric_3;
	fix[20] = antisymmetric_0;
	fix[21] = antisymmetric_1;
	fix[22] = antisymmetric_2;
	fix[23] = antisymmetric_3;
}

Halo::~Halo() {
	// TODO Auto-generated destructor stub
}

void Halo::update(Variable &var) {
	for (int i = 0; i < 4; i++)
		fix[i + ((var.get_boundary() & (15 << (i << 2))) >> (i << 2) << 2)](var);
}
