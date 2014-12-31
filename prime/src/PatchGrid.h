/*
 * patchgrid.h
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#ifndef PATCHGRID_H_
#define PATCHGRID_H_

#include "Grid.h"

/**
 * class PatchGrid extend class Grid by decomposing a domain into smaller
 * Grids with overlap (halo). Y-direction is preferred because if grid is
 * periodic in x (longitude), fftw could be used. This direction should not
 * be decomposed. PatchGrid has a member of a vector of Grid. Grids in
 * this vector will point to member of PatchGrid.
 *
 * e.g.
 * PatchGrid grid("name.lst"); // concrete grid
 * grid = grid.sub(3,3,1); // still concrete grid with vector of
 *                            Grid being abstract.
 */
class PatchGrid: public virtual Grid {
    template<class STREAM>
	friend STREAM &operator<<(STREAM &os, const PatchGrid &grid){
    	os << "---------------PatchGrid Information Begin---------------" << std::endl;
    	os << grid.head_info() << std::endl;
        if (~grid.spec & abstract)
            for (int i = 0; i < grid.ntiles; i++) {
                os << "Tile # " << i << ": " << std::endl;
                os << grid.tile[i];
            }
    	os << "----------------PatchGrid Information End----------------" << std::endl;
    	return os;
    }

protected:
	int ntilex, /**< number of tiles in x-direction */
	ntiley, /**< number of tiles in y direction */
	ntiles; /**< total number of tiles*/
	std::vector<Grid> tile; /**< a tile is a subgrid of Patch */

public:
	PatchGrid();

	/**
	 * This enables conversion from Grid to PatchGrid
	 */
	PatchGrid(const Grid&);

	PatchGrid(const PatchGrid&);

	PatchGrid(const Configure&, GridSpec = abstract);

	PatchGrid& operator=(const PatchGrid&);

	PatchGrid& make();

	PatchGrid& unmake();

	/**
	 * split a grid into multiple tiles, you can call this function
	 * any time and any where in the program because it will only adjust
	 * the pointer and will not allocate/reallocate new arrays.
	 */
	virtual PatchGrid& split(int, int);

	/**
	 * accessing tiles
	 */
	inline const Grid& operator[](int p) const {
		return tile[p];
	}

	inline Grid& operator[](int p) {
		return tile[p];
	}

	std::string head_info() const;

protected:
	PatchGrid& redirect();


};

#endif /* PATCHGRID_H_ */
