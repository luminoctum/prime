/*
 * variable.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: cli
 */

#include "Variable.h"

Variable::Variable() :
		value(0), cur(0), size(0), offset(0), shift1d(0), shift2d(0), shift3d(
				0), name(""), long_name(""), units("") {
}

Variable::Variable(const Variable &var) :
		Grid(var), offset(var.offset), shift1d(var.shift1d), shift2d(
				var.shift2d), shift3d(var.shift3d), name(var.name), long_name(
				var.long_name), units(var.units) {
	redirect();
}

Variable::Variable(const Grid& _grid, std::string _name, std::string _long_name,
		std::string _units, GridSpec _spec) :
		Grid(_grid), name(_name), long_name(_long_name), units(_units) {
	spec = _spec;
	if (~spec & prognostic) nt = 1;
	spec |= abstract;
	set_shift_index(nxh, nxh * nyh, nxh * nyh * nzh);
	redirect();
}

Variable::~Variable() {
	//std::cout << "variable " << name << " deallocated." << std::endl;
	unmake();
}

/**
 * assignment operation changes "value" at "cur" time slice.
 * Other members remain the same. If two variables are of
 * different spatial shape, it will flag an error.
 */
Variable& Variable::operator=(const Variable &var) {
	if (~spec & abstract) {
		if (nz == var.nz && ny == var.ny && nx == var.nx) {
			for (int j = 0; j < ny; j++)
				for (int i = 0; i < nx; i++)
					(*this)(i, j) = var(i, j);
		} else {
			std::cerr << "Variable size does not match" << std::endl;
			exit(1);
		}
	} else {
		Grid::operator=(var);
		set_shift_index(nxh, nxh * nyh, nxh * nyh * nzh);
		redirect();
	}

	return *this;
}

void Variable::make() {
	if (~spec & abstract) return;
	// make Grid
	Grid::make();

	// make Variable
	size = nt * nxh * nyh * nzh;
	value = new FLOAT[size];
	redirect();
}

void Variable::unmake() {
	if (spec & abstract) return;
	// unmake Grid
	Grid::unmake();

	// unmake Variable
	delete[] value;
	redirect();
}

void Variable::redirect() {
	if (spec & abstract) {
		size = 0;
		value = 0;
	}
	cur = 0;
	slice();
}

Variable Variable::sub(int tx, int ty, int p) const {
	Variable result(Grid::sub(tx, ty, p), name, long_name, units, spec);

	return result;
}

Variable& Variable::clear() {
	if (spec & abstract) return *this;
	for (int k = 0; k < nz; k++)
		for (int j = 0; j < ny; j++)
			for (int i = 0; i < nx; i++)
#if defined(DOMAIN_XYZ)
				(*this)(i, j, k) = 0;
#elif defined(DOMAIN_XY)
				(*this)(i, j) = 0;
#elif defined(DOMAIN_YZ)
	(*this)(j, k) = 0;
#elif defined(DOMAIN_Z)
	(*this)(k) = 0;
#else
#endif

	return *this;
}

Variable& Variable::clear_all() {
	if (spec & abstract) return *this;
	for (int i = 0; i < size; i++)
		value[i] = 0.;

	return *this;
}

Variable& Variable::save() {
	if (value == 0) return *this;
	cur = cur % (nt - 1) + 1;
	for (int k = 0; k < nz; k++)
		for (int j = 0; j < ny; j++)
			for (int i = 0; i < nx; i++)
#if defined(DOMAIN_XYZ)
				(*this)(i + cur * shift3d, j, k) = (*this)(i, j, k);
#elif defined(DOMAIN_XY)
				(*this)(i + cur * shift2d, j) = (*this)(i, j);
#elif defined(DOMAIN_YZ)
	(*this)(j + cur * shift2d, k) = (*this)(j, k);
#elif defined(DOMAIN_Z)
	(*this)(k + cur * shift1d) = (*this)(k);
#else
#endif

	return *this;
}

Variable& Variable::set_random_int(int a, int b) {
	if (spec & abstract) return *this;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(a, b);

	for (int k = 0; k < nz; k++)
		for (int j = 0; j < ny; j++)
			for (int i = 0; i < nx; i++)
				(*this)(i, j) = distribution(generator);

	return *this;
}

void Variable::set_shift_index(int s1, int s2, int s3) {
	shift1d = s1;
	shift2d = s2;
	shift3d = s3;
}

Variable& Variable::slice(int t) {
    //std::cerr << nt << std::endl;
	if (t >= nt) {
		std::cerr << "indicated time slice larger than maximum time slice"
				<< std::endl;
		exit(1);
	}
#if defined(DOMAIN_XYZ)
	offset = nh * (1 + shift1d + shift2d) + t * shift3d;
#elif defined(DOMAIN_XY) || defined(DOMAIN_YZ)
	offset = nh * (1 + shift1d) + t * shift2d;
#elif defined(DOMAIN_Z)
	offset = nh + t * shift1d;
#else
#endif

	return *this;
}

std::string Variable::head_info() const {
	std::string result;

	result = "name          : " + name + " (" + long_name + ", " + units
			+ ")\n";
	result += Grid::head_info();

	return result;
}

std::string Variable::value_info() const {
	std::string result;
	int offset;
	char buf[100];

	result = "";
	for (int t = 0; t < nt; t++) {
		offset = t * shift2d;
		sprintf(buf, "t = %d  |", t);
		result += buf;
		for (int i = 0; i < nxh; i++) {
			sprintf(buf, "%8d", i - nh);
			result += buf;
		}
		result += "\n";
		for (int i = -1; i < nxh; i++)
			result += "--------";
		result += "\n";
		for (int j = 0; j < nyh; j++) {
			sprintf(buf, "%7d|", j - nh);
			result += buf;
			for (int i = 0; i < nxh; i++) {
				std::stringstream ss;
				ss << std::setw(8) << value[offset + i + j * shift1d];
				result += ss.str();
				//sprintf(buf, "%8.1e", value[offset + i + j * shift1d]);
				//result += buf;
			}
			result += "\n";
		}
		if (t != nt - 1) result += "\n";
	}

	return result;
}
