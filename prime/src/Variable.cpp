/*
 * variable.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: cli
 */

#include "Variable.h"

Variable::Variable() :
		value(0), cur(0), size(0), offset(0), name(""), long_name(""), units("") {
}

Variable::Variable(const Variable &var) :
		Grid(var), name(var.name), long_name(
                var.long_name), units(var.units) {
    redirect();
}

Variable::Variable(const Grid& _grid, std::string _name, std::string _long_name,
		std::string _units, GridSpec _spec) :
		Grid(_grid), name(_name), long_name(_long_name), units(_units) {
    spec = _spec;
    if (~spec & prognostic)
        nt = 1;
    spec |= abstract;
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
    if (~spec & abstract){
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
        redirect();
    }

	return *this;
}

Variable& Variable::make() {
	if (~spec & abstract)
		return *this;
    // make Grid
	Grid::make();

    // make Variable
	size = nt * nxh * nyh * nzh;
	value = new FLOAT[size];
    redirect();

	return *this;
}

Variable& Variable::unmake() {
	if (spec & abstract)
		return *this;
    // unmake Grid
	Grid::unmake();

    // unmake Variable
	delete[] value;
    redirect();

	return *this;
}

Variable& Variable::redirect() {
	if (spec & abstract) {
		size = 0;
		value = 0;
	}
    cur = 0;
    slice();

	return *this;
}

Variable Variable::sub(int tx, int ty, int p) const {
	Variable result(Grid::sub(tx, ty, p), name, long_name, units,
			spec | abstract);
    result.redirect();

	return result;
}

Variable& Variable::clear() {
    if (spec & abstract) 
        return *this;
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
    if (spec & abstract) 
        return *this;
	for (int i = 0; i < size; i++)
		value[i] = 0.;

	return *this;
}

Variable& Variable::save() {
    if (spec & abstract) 
        return *this;
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
    if (spec & abstract) 
        return *this;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(a, b);

	for (int k = 0; k < nz; k++)
		for (int j = 0; j < ny; j++)
			for (int i = 0; i < nx; i++)
				(*this)(i, j) = distribution(generator);

	return *this;
}

Variable& Variable::slice(int t) {
    if (t >= nt) {
        std::cerr << "indicated time slice larger than maximum time slice" << std::endl;
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

