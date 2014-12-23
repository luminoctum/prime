/*
 * variable.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: cli
 */

#include "Variable.h"

Variable::Variable() :
		value(0), cur(0), size(0), offset(0), name(""), long_name(""), units(""){
}

Variable::Variable(const Grid& _grid, std::string _name, std::string _long_name,
		std::string _units, GridSpec _spec) :
		Grid(_grid), name(_name), long_name(_long_name), units(_units) {
	spec = _spec;
	if (~spec & prognostic)
		nt = 1;
	if (spec & abstract) {
		redirect();
	} else {
		spec |= abstract;
		make();
	}
	set_offset();
}

Variable::Variable(const Variable &var) :
		Grid(var), cur(0), offset(var.offset), name(var.name), long_name(
				var.long_name), units(var.units) {
	if (spec & abstract) {
		size = 0;
		value = var.value;
	} else {
		size = nt * nxh * nyh * nzh;
		value = new FLOAT[size];
		memcpy(value, var.value, size * sizeof(FLOAT));
	}
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
	if (this == &var)
		return *this;
	if (nz == var.nz && ny == var.ny && nx == var.nx)
		for (int k = 0; k < nz; k++) {
			for (int j = 0; j < ny; j++)
				for (int i = 0; i < nx; i++)
#if defined(DOMAIN_XYZ)
					(*this)(i, j, k) = var(i, j, k);
#elif defined(DOMAIN_XY)
					(*this)(i, j) = var(i, j);
#elif defined(DOMAIN_YZ)
			(*this)(j, k) = var(j, k);
#elif defined(DOMAIN_Z)
			(*this)(k) = var(k);
#else
#endif
		}
	else {
		std::cerr << "cannot assign a variable to a different spatial shape"
				<< std::endl;
		exit(1);
	}
	return *this;
}

Variable& Variable::operator=(FLOAT ff) {
	for (int k = 0; k < nz; k++)
		for (int j = 0; j < ny; j++)
			for (int i = 0; i < nx; i++)
#if defined(DOMAIN_XYZ)
				(*this)(i, j, k) = ff;
#elif defined(DOMAIN_XY)
				(*this)(i, j) = ff;
#elif defined(DOMAIN_YZ)
	(*this)(j, k) = ff;
#elif defined(DOMAIN_Z)
	(*this)(k) = ff;
#else
#endif
	return *this;
}

Variable& Variable::make() {
	if (~spec & abstract)
		return *this;
	Grid::make();
	size = nt * nxh * nyh * nzh;
	value = new FLOAT[size];

	return *this;
}

Variable& Variable::unmake() {
	if (spec & abstract)
		return *this;
	Grid::unmake();
	size = 0;
	delete[] value;
	redirect();

	return *this;
}

Variable& Variable::redirect() {
	Grid::redirect();
	if (spec & abstract) {
		size = 0;
		value = 0;
	}

	return *this;
}

Variable Variable::sub(int tx, int ty, int p) const{
	Variable result(Grid::sub(tx, ty, p), name, long_name, units, spec | abstract);
	result.set_offset();

	return result;
}

Variable& Variable::clear() {
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
	for (int i = 0; i < size; i++)
		value[i] = 0.;

	return *this;
}

Variable& Variable::save() {
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
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(a, b);

	for (int k = 0; k < nz; k++)
		for (int j = 0; j < ny; j++)
			for (int i = 0; i < nx; i++)
				(*this)(i, j) = distribution(generator);

	return *this;
}

Variable& Variable::set_offset(int t) {
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

