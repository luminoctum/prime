/*
 * boundary.h
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#ifndef BOUNDARY_H_
#define BOUNDARY_H_

#include "Variable.h"


// BoundaryType = 0x0;
void none_0(Variable &var);

// BoundaryType = 0x1;
void periodic_0(Variable&);
void periodic_1(Variable&);
void periodic_2(Variable&);
void periodic_3(Variable&);

// BoundaryType = 0x2;
void zero_gradient_0(Variable&);
void zero_gradient_1(Variable&);
void zero_gradient_2(Variable&);
void zero_gradient_3(Variable&);

// BoundaryType = 0x3;
void fixed_gradient_0(Variable&);
void fixed_gradient_1(Variable&);
void fixed_gradient_2(Variable&);
void fixed_gradient_3(Variable&);

// BoundaryType = 0x4;
void symmetric_0(Variable&);
void symmetric_1(Variable&);
void symmetric_2(Variable&);
void symmetric_3(Variable&);

// BoundaryType = 0x5;
void antisymmetric_0(Variable&);
void antisymmetric_1(Variable&);
void antisymmetric_2(Variable&);
void antisymmetric_3(Variable&);

typedef void (*BoundaryFunc)(Variable&);

#endif /* BOUNDARY_H_ */
