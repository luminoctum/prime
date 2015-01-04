/*
 * grid.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: cli
 */

#include "Grid.h"

Grid::Grid() :
		lonbot(0), lontop(0), dlon(0), dx(0), latbot(0), lattop(0), dlat(0), dy(
				0), pbot(0), ptop(0), pref(0), dlnp(0), dz(0), radius0(0), lat0(
				0), lon(0), rlon(0), mlon(0), lat(0), rlat(0), mlat(0), ptol(0), nt(
				0), nx(0), ny(0), nz(0), nh(0), nxh(0), nyh(0), nzh(0), spec(abstract), boundary { { 0, 0,
				0, 0, 0, 0 } } {
}

Grid::Grid(const Grid &grid) :
		lonbot(grid.lonbot), lontop(grid.lontop), dlon(grid.dlon), dx(grid.dx), latbot(
				grid.latbot), lattop(grid.lattop), dlat(grid.dlat), dy(grid.dy), pbot(
				grid.pbot), ptop(grid.ptop), pref(grid.pref), dlnp(grid.dlnp), dz(
				grid.dz), radius0(grid.radius0), lat0(grid.lat0), nt(grid.nt), nx(
				grid.nx), ny(grid.ny), nz(grid.nz), nh(grid.nh), nxh(grid.nxh), nyh(
				grid.nyh), nzh(grid.nzh), spec(grid.spec), boundary(
				grid.boundary) {
	//std::cout << "Grid copy constructor called." << std::endl;
	spec |= abstract;
    redirect();
}

Grid::Grid(const Configure& config, GridSpec gs) :
		spec(gs), boundary { { 0, 0, 0, 0, 0, 0 } } {
    spec |= abstract;
    redirect();
	config.get_entry("lowest_latitude", &latbot);
	config.get_entry("highest_latitude", &lattop);
	config.get_entry("lowest_longitude", &lonbot);
	config.get_entry("highest_longitude", &lontop);
	config.get_entry("bottom_pressure", &pbot);
	config.get_entry("top_pressure", &ptop);
	config.get_entry("reference_pressure", &pref);
	config.get_entry("number_of_latitude_grids", &ny);
	config.get_entry("number_of_longitude_grids", &nx);
	config.get_entry("number_of_vertical_layers", &nz);
	config.get_entry("mean_radius", &radius0);
	config.get_entry("mean_latitude", &lat0);

	nt = 2;
	dlon = (lontop - lonbot) / (nx - 1);
	dlat = (lattop - latbot) / (ny - 1);
	dlnp = log(ptop / pbot) / (nz - 1);
	dx = dlon * radius0 * cos(lat0 * DEG_TO_RAD);
	dy = dlat * radius0 * DEG_TO_RAD;
	dz = 1.;
	nh = 2;
	nxh = nx == 1 ? 1 : nx + 2 * nh;
	nyh = ny == 1 ? 1 : ny + 2 * nh;
	nzh = nz == 1 ? 1 : nz + 2 * nh;

	if (latbot == -90.)
		spec |= isspole;
	if (lattop == 90.)
		spec |= isnpole;

	boundary = { {1,2,4,5,0,0}};
}

Grid::~Grid() {
	//std::cout << "Grid deallocated" << std::endl;
	unmake();
}

Grid& Grid::operator=(const Grid &grid) {
	//std::cout << "Grid assignment operator is called" << std::endl;
	if (this == &grid)
		return *this;
	lonbot = grid.lonbot;
	lontop = grid.lontop;
	dlon = grid.dlon;
	dx = grid.dx;
	latbot = grid.latbot;
	lattop = grid.lattop;
	dlat = grid.dlat;
	dy = grid.dy;
	pbot = grid.pbot;
	ptop = grid.ptop;
	pref = grid.pref;
	dlnp = grid.dlnp;
	dz = grid.dz;
	radius0 = grid.radius0;
	lat0 = grid.lat0;
	nt = grid.nt;
	nx = grid.nx;
	ny = grid.ny;
	nz = grid.nz;
	nh = grid.nh;
	nxh = grid.nxh;
	nyh = grid.nyh;
	nzh = grid.nzh;
	boundary = grid.boundary;

    if (~spec & abstract){
        unmake();
        make();
    }

	return *this;
}

void Grid::make() {
	if (~spec & abstract) return;
	lon = new FLOAT[nx];
	rlon = new FLOAT[nx];
	mlon = new FLOAT[nx];
	lat = new FLOAT[ny];
	rlat = new FLOAT[ny];
	mlat = new FLOAT[ny];
	ptol = new FLOAT[nz];

	for (int i = 0; i < nx; i++) {
		lon[i] = lonbot + i * dlon;
		rlon[i] = 1.;
		mlon[i] = 1.;
	}
	for (int i = 0; i < ny; i++) {
		lat[i] = latbot + i * dlat;
		rlat[i] = 1.;
		mlat[i] = 1.;
	}
	for (int i = 0; i < nz; i++) {
		ptol[i] = pbot * exp(i * dlnp);
	}

	spec &= ~abstract;
}

void Grid::unmake() {
	if (spec & abstract) return;
	//std::cout << "grid unmade" << std::endl;
	delete[] lon;
	delete[] rlon;
	delete[] mlon;
	delete[] lat;
	delete[] rlat;
	delete[] mlat;
	delete[] ptol;
	spec |= abstract;
	redirect();
}

Grid Grid::sub(int tx, int ty, int p) const {
    //std::cout << tx << " " << ty << " " << p << std::endl;
    //std::cout << nx << " " << ny << std::endl;
	if ((tx < 1) | (tx > nx) | (ty < 1) | (ty > ny) | (p < 0)
			| (p >= tx * ty)) {
		std::cerr
				<< "Parameter out of range : 1 <= tx, ty <= nx, ny   0 <= p < tx * ty"
				<< std::endl;
		exit(1);
	}
	int sx = nx / tx, sy = ny / ty;
	int i = p % tx, j = p / tx;

	Grid result(*this);

	result.lonbot = lonbot + i * sx * dlon;
	result.latbot = latbot + j * sy * dlat;
	result.lontop =
			(i == tx - 1 ? lontop : lonbot + (i + 1) * sx * dlon - dlon);
	result.lattop =
			(j == ty - 1 ? lattop : latbot + (j + 1) * sy * dlat - dlat);
	result.nx = (i == tx - 1 ? sx + nx % tx : sx);
	result.ny = (j == ty - 1 ? sy + ny % ty : sy);
	result.nxh = result.nx + 2 * result.nh;
	result.nyh = result.ny + 2 * result.nh;

	if (i != 0)
		result.boundary[0] = 0;
	if (i != tx - 1)
		result.boundary[1] = 0;
	if (j != 0)
		result.boundary[2] = 0;
	if (j != ty - 1)
		result.boundary[3] = 0;

	return result;
}

void Grid::redirect() {
	if (spec & abstract) {
		lon = 0;
		rlon = 0;
		mlon = 0;
		lat = 0;
		rlat = 0;
		mlat = 0;
	}
}


std::string Grid::head_info() const{
	std::string result;
	char buf[100];

	result = "";
	sprintf(buf, "%-14s: [%8.2f : %8.2f : %8.2f] ", "longitude", lonbot, dlon, lontop);
	result += buf;
	sprintf(buf, "nx = %5d, nxh = %5d, dx = %12.2f km\n", nx, nxh, dx / 1.E3);
	result += buf;
	sprintf(buf, "%-14s: [%8.2f : %8.2f : %8.2f] ", "latitude", latbot, dlat, lattop);
	result += buf;
	sprintf(buf, "ny = %5d, nyh = %5d, dy = %12.2f km\n", ny, nyh, dy / 1.E3);
	result += buf;
	sprintf(buf, "%-14s: [%8.2e : %8.2f : %8.2e] ", "vertical", pbot, exp(dlnp), ptop);
	result += buf;
	sprintf(buf, "nz = %5d, nzh = %5d, dz = %12.2f km\n", nz, nzh, dz / 1.E3);
	result += buf;
	sprintf(buf, "%-14s: %d\n", "timesteps", nt);
	result += buf;
	sprintf(buf, "%-14s: %d %d %d %d %d %d\n", "boundary",
			boundary[0], boundary[1], boundary[2], boundary[3], boundary[4], boundary[5]);
    result += buf;
	std::stringstream ss;
	ss << spec;
	result += ss.str();
	result += "\n";

	return result;
}
