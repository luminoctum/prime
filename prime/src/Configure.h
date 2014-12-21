/*
 * configure.h
 *
 *  Created on: Dec 15, 2014
 *      Author: cli
 */

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "constants.h"

/**
 * Configure class manages the configuration of the model
 */
class Configure {
	friend std::ostream &operator<<(std::ostream &, Configure &);

public:
	int opcount, /**< number of configuration entries */
	opname_size, /**< size of option name array */
	opchoice_size, /**< size of option choice array */
	opvalue_size, /**< size of option value array */
	*opid; /**< option id */
	char **opname, /**< array of option names */
	**opchoice, /**< array of option choices */
	**opvalue; /**< array of option value */
protected:
	int max_size;
	char *opname_store, *opchoice_store, *opvalue_store;
public:

	Configure(char const *);

	Configure(const Configure&);

	~Configure();

	Configure& operator=(const Configure&);

	void get_entry(char const *, int *) const;

	void get_entry(char const *, float *) const;

	void get_entry(char const *, double *) const;

	void get_entry(char const *, char *) const;

protected:
	void read_from_file(char const *);

	void add_entry(const char *);

	void preallocate_memory();

	void allocate_memory();

	void clear_memory();

	void complete();
};

#endif /* CONFIGURE_H_ */
