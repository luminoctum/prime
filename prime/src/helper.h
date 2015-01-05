/*
 * helper.h
 *
 *  Created on: Dec 15, 2014
 *      Author: cli
 */

#ifndef HELPER_H_
#define HELPER_H_

#include "constants.h"

/**
 * The enum type given to the constructors of Vairable, telling whether this variable is a concrete, prognostic, etc.
 * a concrete variable allocates space for value.
 * an abstract variable does not allocates space for value, instead value points to somewhere else
 * a prognostic variable has several time steps stored and has a tendency term
 * a diagnostic variable has only one time step.
 * This enum type is constructed based on deallii
 * check "UpdateFlags" for a complete reference:
 * http://www.dealii.org/8.1.0/doxygen/deal.II/group__feaccess.html#ga8c065f9a62f55fd54c095a21d157ba93
 * default GridSpec is concrete, diagnostic, nonstag
 */
enum GridSpec {
	abstract = 0x001,
	prognostic = 0x002,
	stagx = 0x004,
	stagy = 0x008,
	stagz = 0x010,
	isspole = 0x020,
	isnpole = 0x040
};

template<class STREAM>
STREAM &operator<<(STREAM &os, GridSpec vt) {
	os << "gridspec      : |";
	if (vt & abstract)
		os << "abstract|";
	else
		os << "concrete|";
	if (vt & prognostic)
		os << "prognostic|";
	else
		os << "diagnostic|";
	if (vt & stagx)
		os << "stag-x|";
	else
		os << "center-x|";
	if (vt & stagy)
		os << "stag-y|";
	else
		os << "center-y|";
	if (vt & stagz)
		os << "stag-z|";
	else
		os << "center-z|";

	return os;
}

/**
 * Global operator which returns an object in which all bits are set
 * which are either set in the first or the second argument. This
 * operator exists since if it did not then the result of the bit-or
 * <tt>operator |</tt> would be an integer which would in turn trigger
 * a compiler warning when we tried to assign it to an object of type
 * GridSpec.
 *
 * @ref GridSpec
 */
inline GridSpec operator |(GridSpec vt1, GridSpec vt2) {
	return static_cast<GridSpec>(static_cast<unsigned int>(vt1)
			| static_cast<unsigned int>(vt2));
}

/**
 * bit-and operator, same as above
 */
inline GridSpec operator &(GridSpec vt1, GridSpec vt2) {
	return static_cast<GridSpec>(static_cast<unsigned int>(vt1)
			& static_cast<unsigned int>(vt2));
}

/**
 * bit-not operator, same as above
 */
inline GridSpec operator ~(GridSpec vt) {
	return static_cast<GridSpec>(~static_cast<unsigned int>(vt));
}

/**
 * Global operator which clears all the bits in the first argument if
 * they are not also set in the second argument
 */
inline GridSpec& operator &=(GridSpec &vt1, GridSpec vt2) {
	vt1 = vt1 & vt2;
	return vt1;
}

/**
 * Global operator which sets all the bits in the first argument if
 * they either appears in the first argument or the second argument
 */
inline GridSpec& operator |=(GridSpec &vt1, GridSpec vt2) {
	vt1 = vt1 | vt2;
	return vt1;
}

/**
 * math functions
 */
void tridiag_solver(long n, FLOAT *a, FLOAT *b, FLOAT *c, FLOAT *r, FLOAT *v);


#endif /* HELPER_H_ */
