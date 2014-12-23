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
	friend STREAM &operator<<(STREAM &os, const Variable &var){
    	os << "-------------------Variable Information Begin-----------------"
    			<< std::endl;
    	os << "name: " << var.name << std::endl << "long name : " << var.long_name
    			<< std::endl << "units : " << var.units << std::endl << "nx = "
    			<< var.nx << " ny = " << var.ny << " nz = " << var.nz << " nt = "
    			<< var.nt << " nh = " << var.nh << std::endl << "nxh = " << var.nxh
    			<< " nyh = " << var.nyh << " nzh = " << var.nzh << " offset = "
    			<< var.offset << std::endl;
    	os << var.spec << std::endl;
    	int offset;
    	if (var.value == 0) {
    		os << "-------------------Variable Information End-----------------"
    				<< std::endl;
    		return os;
    	}
    #if defined(DOMAIN_XY)
    	for (int t = 0; t < var.nt; t++) {
    		offset = t * var.shift2d;
    		os << std::setw(7) << t << "|";
    		for (int i = 0; i < var.nxh; i++)
    			os << std::setw(8) << i - var.nh;
    		os << std::endl;
    		for (int i = -1; i < var.nxh; i++)
    			os << "--------";
    		os << std::endl;
    		for (int j = 0; j < var.nyh; j++) {
    			for (int i = -1; i < var.nxh; i++)
    				if (i == -1)
    					os << std::setw(7) << j - var.nh << "|";
    				else
    					os << std::setw(8)
    							<< var.value[offset + i + j * var.shift1d];
    			os << std::endl;
    		}
    		os << std::endl;
    	}
    #endif
    	os << "-------------------Variable Information End-----------------"
    			<< std::endl;
    	return os;
    }

public:
	FLOAT *value; /**< variable value */

protected:
	int cur, /**< current storage of time slice */
	size, /**< total number of elements in value, size of an abstract variable is 0 */
	offset; /**< offset value used to access value at different time storage */

	std::string name, /**< variable short name */
	long_name, /**< variable long name */
	units; /**< variable units */

public:
	Variable();

	Variable(const Variable&);

	Variable(const Grid&, std::string = "", std::string = "", std::string = "",
			GridSpec = defaultgrid);

	~Variable();

	Variable& operator=(const Variable&);

	// set value to a FLOAT
	Variable& operator=(FLOAT);

	Variable& make();

	Variable& unmake();

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

	Variable& set_offset(int = 0);

	Variable& set_random_int(int = -10, int = 10);

	Variable& set_random_float();

#if defined(DOMAIN_XYZ)
	inline FLOAT& operator()(int i, int j, int k) const {
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
	inline FLOAT& operator()(int k) const {
		return value[offset + k];
	}
#else
#endif

protected:
	Variable& redirect();

};

#endif /* VARIABLE_H_ */
