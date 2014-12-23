/*
 * grid.h
 *
 *  Created on: Dec 15, 2014
 *      Author: cli
 */

#ifndef GRID_H_
#define GRID_H_

#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "Configure.h"
#include "helper.h"

/**
 * class Grid manages the discretization and geometry of the model
 * Grid constructors will return a concrete spec. Assigment operator and
 * copy constructor will keep the spec of the grid. Member function sub will return an abstract grid
 * because grid has all the information (scalar quantities) it
 * need to create a concrete grid. It is not necessary to allocate
 * spaces. Grid wants to minimize its space usage. User can create
 * a concrete grid by "make" when they want to.  make and unmake will change
 * the abstract spec of Grid.
 *
 * e.g.
 * Grid grid("name.lst"); // concrete grid
 * grid = grid.sub(3,3,1); // abstract grid with scalar
 *                            member updated.
 *
 */
class Grid {
    template<class STREAM>
	friend STREAM &operator<<(STREAM &os, const Grid &grid){
    	os << "-----------Grid Information Begin-----------"
    			<< std::endl;
    	os << "longitude: [" << grid.lonbot << ":" << grid.dlon << ":"
    			<< grid.lontop << "], nx = " << grid.nx << ", nxh = " << grid.nxh
    			<< ", dx = " << grid.dx / 1.E3 << " km" << std::endl;
    	os << "latitude: [" << grid.latbot << ":" << grid.dlat << ":" << grid.lattop
    			<< "], ny = " << grid.ny << ", nyh = " << grid.nyh << ", dy = "
    			<< grid.dy / 1.E3 << " km" << std::endl;
    	os << "vertical: Log[" << grid.pbot << ":" << exp(grid.dlnp) << ":"
    			<< grid.ptop << "], nz = " << grid.nz << ", nzh = " << grid.nzh
    			<< ", dz = " << grid.dz / 1.E3 << " km" << std::endl;
    	os << "number of time steps stored: " << grid.nt << std::endl;
    	os << "Grid specifics: " << grid.spec << std::endl;
    	os << "Boundary condition: ";
    	for (int i = 0; i < 6; i++)
    		os << grid.boundary[i] << " ";
    	os << std::endl
    			<< "-----------Grid Information End-------------"
    			<< std::endl;

    	return os;
    }

public:
	FLOAT lonbot, /**< bottom longitude */
	lontop, /**< top longitude */
	dlon, /**< longitude grid spacing, deg */
	dx, /**< longitude grid spacing, m */
	latbot, /**< bottom latitude */
	lattop, /**< top latitude */
	dlat, /**< latitude grid spacing, deg */
	dy, /**< latitude grid spacing, m */
	pbot, /**< bottom pressure */
	ptop, /**< top pressure */
	pref, /**< reference pressure */
	dlnp, /**< log-pressure spacing, deg */
	dz, /**< vertical spacing, m */
	radius0, /**< mean radius */
	lat0, /**< mean latitude */
	*lon, /**< longitude, deg */
	*rlon, /**< longitude map factor, r */
	*mlon, /**< longitude map factor, 1/(r*dlon*DEG) */
	*lat, /**< latitude, deg */
	*rlat, /**< latitude map factor, R */
	*mlat, /**< latitude map factor, 1/(r*dlat*DEG) */
	*ptol; /**< total pressure, pa */
	//*mmlatlon;  /**< mlon * mlat */
	//*mask     //TODO mask array that determines topology type and domain nesting

	int nt, /**< number of saved previous time steps,
	 This member is supposed to be in variable
	 However variable does not read namelist and
	 this member is initialized from namelist.
	 Therefore it is in the Grid*/
	nx, /**< number of grids in longitude */
	ny, /**< number of grids in latitude */
	nz, /**< number of vertial layers */
	nh, /**< halo width */
	nxh, /**< nx + 2 * nh */
	nyh, /**< ny + 2 * nh */
	nzh; /**< nz + 2 * nh */

protected:
	int shift1d, /**< index shift in first dimension,
	 usually shift1d = nxh */
	shift2d, /**< index shift in first two dimension,
	 usually shift2d = nxh * nyh*/
	shift3d; /**< index shift in first 3 dimension,
	 usually shift3d = nxh * nyh * nzh*/

	GridSpec spec; /**< specifics of the grid **/

	/*BoundaryType boundary;  a 6-bytes binary number that
	 represent boundary condition at 6 faces
	 to get the value of k-th face, use
	 (boundary & (15 << 4*k)) >> 4*k */
    std::array<int, 6> boundary;

public:
	/**
	 * usually, default constructor is not used. However, quite a few standard library
	 * constructs mandate default constructible objects, iterators, resize, etc. This default
	 * constructors will initialize pointers to NULL and numerical values to zero.
	 */
	Grid();

	Grid(const Grid&);

	virtual ~Grid();

	Grid(const Configure&, GridSpec = defaultgrid);

	/**
	 * return an abstract grid that equal to the original grid.
	 */
	Grid& operator=(const Grid&);

	/**
	 * allocate the memory in the grid
	 */
	virtual Grid& make();

	/**
	 * deallocate the memory in the grid
	 */
	virtual Grid& unmake();

	/**
	 * return an abstract subgrid of the current grid.
	 */
	Grid sub(int, int, int) const;

	void set_shift_index(int s1, int s2, int s3) {
		shift1d = s1;
		shift2d = s2;
		shift3d = s3;
	}

    void set_boundary(std::array<int, 6> b){
        boundary = b;
    }

	int get_boundary(int i) const{
		return boundary[i];
	}

protected:

	/**
	 * redirect pointers in abstract object to zero
	 */
	virtual Grid& redirect();

};

#endif /* GRID_H_ */
