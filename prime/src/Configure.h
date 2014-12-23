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
    template<class STREAM>
	friend STREAM &operator<<(STREAM & os, Configure & co){
    	os << "Total number of enties: " << co.opcount << std::endl
    			<< "Size of option name array: " << co.opname_size << std::endl
    			<< "Size of option choice array: " << co.opchoice_size << std::endl
    			<< "Size of option value array: " << co.opvalue_size << std::endl;
    	for (int i = 0; i <= 100; i++)
    		printf("-");
    	std::cout << std::endl;
    	printf("|  %-38s%-20s%-40s|\n", "Entry name", "Entry choice",
    			"Entry value");
    	for (int i = 0; i <= 100; i++)
    		printf("-");
    	std::cout << std::endl;
    	for (int i = 0; i < co.opcount; i++)
    		printf("|  %-38s%-20d%-40s|\n", co.opname[i], co.opid[i],
    				co.opvalue[i]);
    	for (int i = 0; i <= 100; i++)
    		printf("-");
    	std::cout << std::endl;
    	return os;
    };

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
