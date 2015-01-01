/*
 * variable.h
 *
 *  Created on: Dec 16, 2014
 *      Author: cli
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "constants.h"
#include "helper.h"
#include "Grid.h"

/**
 * Variable allocated an array on Grid
 */
class Variable: public virtual Grid {
	template<class STREAM>
	friend STREAM &operator<<(STREAM &os, const Variable &var) {
		os << "-------------------Variable Information Begin-----------------"
				<< std::endl;
		os << var.head_info();
		//if (~var.spec & abstract) os << var.value_info();
		if (var.value != 0) os << var.value_info();
		os << "-------------------Variable Information End-----------------"
				<< std::endl;
		return os;
	}

public:
	FLOAT *value; /**< variable value */

protected:
	int cur, /**< current storage of time slice */
	size, /**< total number of elements in value, size of an abstract variable is 0 */
	offset, /**< offset value used to access value at different time storage */
	shift1d, /**< index shift in first dimension, usually shift1d = nxh */
	shift2d, /**< index shift in first two dimension, usually shift2d = nxh * nyh*/
	shift3d; /**< index shift in first 3 dimension, usually shift3d = nxh * nyh * nzh*/

	std::string name, /**< variable short name */
	long_name, /**< variable long name */
	units; /**< variable units */

public:
	Variable();

	Variable(const Variable&);

	Variable(const Grid&, std::string = "", std::string = "", std::string = "",
			GridSpec = abstract);

	~Variable();

	Variable& operator=(const Variable&);

	void make();

	void unmake();

	Variable sub(int, int, int) const;

	/**
	 * set value in the current time to zero, exclude boundary values
	 * Though clear doesn't change any member in Variable, we cannot make
	 * it return a constant referece because we maybe want to nest expression
	 * and later functions might change variable.
	 */
	Variable& clear();

	/**
	 * set all value to zero, include boundary values
	 */
	Variable& clear_all();

	/**
	 * save value at current time, time index would loop to begin
	 */
	Variable& save();

	Variable& slice(int = 0);

	Variable& set_random_int(int = -10, int = 10);

	void set_shift_index(int s1, int s2, int s3);

#if defined(DOMAIN_XYZ)
	inline FLOAT& operator()(int i, int j, int k) {
		return value[offset + i + j * shift1d + k * shift2d];
	}
#elif defined(DOMAIN_XY) || defined(DOMAIN_YZ)
	inline const FLOAT& operator()(int i, int j) const {
		return value[offset + i + j * shift1d];
	}
	inline FLOAT& operator()(int i, int j) {
		return value[offset + i + j * shift1d];
	}
#elif defined(DOMAIN_Z)
	inline FLOAT& operator()(int k) {
		return value[offset + k];
	}
#else
#endif

	std::string head_info() const;

	std::string value_info() const;

protected:
	void redirect();

};

#endif /* VARIABLE_H_ */
