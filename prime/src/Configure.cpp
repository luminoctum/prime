/*
 * configure.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: cli
 */

#include "Configure.h"

Configure::Configure(char const *inpfile) :
		opcount(0), opname_size(0), opchoice_size(0), opvalue_size(0), max_size(
		MAX_CHAR_LENGTH) {
	preallocate_memory();
	read_from_file(inpfile);
}

Configure::Configure(const Configure &co) :
		opcount(co.opcount), opname_size(co.opname_size), opchoice_size(
				co.opchoice_size), opvalue_size(co.opvalue_size), max_size(
				co.max_size) {
	//std::cout << "Configure copy constructor is called" << std::endl;

	preallocate_memory();

	memcpy(opid, co.opid, opcount * sizeof(int));
	memcpy(opname_store, co.opname_store, opname_size * sizeof(char));
	memcpy(opchoice_store, co.opchoice_store, opchoice_size * sizeof(char));
	memcpy(opvalue_store, co.opvalue_store, opvalue_size * sizeof(char));

	for (int i = 0; i < opcount; i++) {
		opname[i] = opname_store + int(co.opname[i] - co.opname[0]);
		opchoice[i] = opchoice_store + int(co.opchoice[i] - co.opchoice[0]);
		opvalue[i] = opvalue_store + int(co.opvalue[i] - co.opvalue[0]);
	}
}

Configure::~Configure() {
	//std::cout << "Configure destructor is called" << std::endl;
	clear_memory();
}

Configure& Configure::operator=(const Configure &co) {
	//std::cout << "Configure assignment operator is called" << std::endl;
	if (this == &co)
		return *this;
	opcount = co.opcount;
	max_size = co.max_size;
	opname_size = co.opname_size;
	opchoice_size = co.opchoice_size;
	opvalue_size = co.opvalue_size;

	clear_memory();
	preallocate_memory();

	memcpy(opid, co.opid, opcount * sizeof(int));
	memcpy(opname_store, co.opname_store, opname_size * sizeof(char));
	memcpy(opchoice_store, co.opchoice_store, opchoice_size * sizeof(char));
	memcpy(opvalue_store, co.opvalue_store, opvalue_size * sizeof(char));

	for (int i = 0; i < opcount; i++) {
		opname[i] = opname_store + int(co.opname[i] - co.opname[0]);
		opchoice[i] = opchoice_store + int(co.opchoice[i] - co.opchoice[0]);
		opvalue[i] = opvalue_store + int(co.opvalue[i] - co.opvalue[0]);
	}

	return *this;
}

void Configure::get_entry(char const *name, int *value) const {
	bool find_entry = false;
	for (int i = 0; i < opcount; i++) {
		if (!strcmp(opname[i], name)) {
			find_entry = true;
			sscanf(opvalue[i], "%d", value);
			break;
		}
	}
	if (!find_entry) {
		std::cout << "Cannot find entry: " << name << std::endl;
		std::exit(1);
	}
}

void Configure::get_entry(char const *name, float *value) const {
	bool find_entry = false;
	for (int i = 0; i < opcount; i++) {
		if (!strcmp(opname[i], name)) {
			find_entry = true;
			sscanf(opvalue[i], "%f", value);
			break;
		}
	}
	if (!find_entry) {
		std::cout << "Cannot find entry: " << name << std::endl;
		std::exit(1);
	}
}

void Configure::get_entry(char const *name, double *value) const {
	bool find_entry = false;
	for (int i = 0; i < opcount; i++) {
		if (!strcmp(opname[i], name)) {
			find_entry = true;
			sscanf(opvalue[i], "%lf", value);
			break;
		}
	}
	if (!find_entry) {
		std::cout << "Cannot find entry:" << name << std::endl;
		std::exit(1);
	}
}

void Configure::get_entry(char const *name, char *value) const {
	bool find_entry = false;
	for (int i = 0; i < opcount; i++) {
		if (!strcmp(opname[i], name)) {
			find_entry = true;
			sscanf(opvalue[i], "\"%s\"", value);
			break;
		}
	}
	if (!find_entry) {
		std::cout << "Cannot find entry: " << name << std::endl;
		std::exit(1);
	}
}

void Configure::read_from_file(char const *inpfile) {
	FILE *fp = fopen(inpfile, "r");
	char *cstack, entry[MAX_CHAR_LENGTH];
	long size;
	int ich, i = 0, j = 0, line = 0;
	/* check file exist */
	if (!fp) {
		printf("Cannot open file %s ...\n", inpfile);
		exit(1);
	}
	/* check file size */
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	cstack = new char[size + 1];
	/* rewind to the begining */
	fseek(fp, 0, SEEK_SET);
	while ((ich = fgetc(fp)) != EOF)
		cstack[i++] = (char) ich;
	cstack[i] = '\0';
	fclose(fp);
	j = 0;
	for (i = 0; i < size; i++) {
		/* strip off comments */
		if (cstack[i] == '/' && cstack[i + 1] == '/')
			while (i < size && cstack[i] != '\n')
				if (cstack[i++] == '\n')
					line++;
		if (cstack[i] == '/' && cstack[i + 1] == '*')
			while (i < size && !(cstack[i - 2] == '*' and cstack[i - 1] == '/'))
				if (cstack[i++] == '\n')
					line++;
		if (i == size) {
			printf("Comment style error at line %d ...\n", line);
			exit(1);
		}
		/* extract one entry */
		if (cstack[i] != ';')
			entry[j++] = cstack[i];
		else {
			entry[j] = '\0';
			add_entry(entry);
			j = 0;
		}
	}
	delete[] cstack;
	complete();
}

void Configure::add_entry(const char *entry) {
	int cur, nchoice;
	bool braced = false;
	const char *pchar, *pcolon;
	char entry_nospace[MAX_CHAR_LENGTH];
	/* strip off white space characters except those are braced */
	pchar = entry;
	cur = 0;
	while (*pchar != '\0') {
		if (braced && *pchar == '"') {
			braced = false;
			entry_nospace[cur++] = *pchar++;
			continue;
		}
		if (!braced && *pchar == '"') {
			braced = true;
			entry_nospace[cur++] = *pchar++;
			continue;
		}
		if (!braced && (*pchar == '\n' || *pchar == '\t' || *pchar == ' ')) {
			pchar++;
			continue;
		}
		entry_nospace[cur++] = *pchar++;
	}
	entry_nospace[cur] = '\0';

	/* extract option name */
	cur = opname_size;
	opname[opcount] = opname_store + cur;
	pchar = entry_nospace;
	while (*pchar != '[' && *pchar != '\0') {
		opname_size++;
		opname_store[cur++] = *pchar++;
	}
	if (*pchar == '\0') {
		std::cout << "Error exists in reading entry: " << entry << std::endl;
		std::exit(1);
	}
	opname_store[cur] = '\0';
	opname_size++;

	/* extract option choice */
	cur = opchoice_size;
	opchoice[opcount] = opchoice_store + cur;
	pchar++;
	braced = false;
	nchoice = 1;

	while (*pchar != ']' && *pchar != '\0') {
		opchoice_size++;
		if (braced && *pchar == '"') {
			braced = false;
			opchoice_store[cur++] = *pchar++;
			continue;
		}
		if (!braced && *pchar == '"') {
			braced = true;
			opchoice_store[cur++] = *pchar++;
			continue;
		}
		if (!braced && *pchar == ',')
			nchoice++;
		opchoice_store[cur++] = *pchar++;
	}
	if (*pchar == '\0') {
		std::cout << "Error exists in reading choice: " << entry << std::endl;
		std::exit(1);
	}
	opchoice_store[cur] = '\0';
	opchoice_size++;

	/* extract option id and value*/
	cur = opvalue_size;
	opvalue[opcount] = opvalue_store + cur;
	pchar++;
	if (*pchar != '=') {
		std::cout << "Error exists in reading value: " << entry << std::endl;
		std::exit(1);
	}
	pchar++;
	opid[opcount] = 0;
	pcolon = strchr(pchar, ':');
	if (nchoice > 1 && pcolon != NULL) {
		while (pchar != pcolon) {
			if (*pchar > '9' || *pchar < '0') {
				std::cout << "Error exists in reading option: " << entry
						<< std::endl;
				std::exit(1);
			}
			opid[opcount] = 10 * opid[opcount] + (int) (*pchar) - 48;
			pchar++;
		}
		if (opid[opcount] >= nchoice) {
			std::cout << "Option id larger than total amount of options in : "
					<< std::endl;
			std::exit(1);
		}
		pchar++;
		while (*pchar != '\0') {
			opvalue_size++;
			opvalue_store[cur++] = *pchar++;
		}
	} else if (nchoice > 1 && pcolon == NULL) {
		while (*pchar != '\0') {
			if (*pchar > '9' || *pchar < '0') {
				std::cout << "Error exists in reading option: " << entry
						<< std::endl;
				std::exit(1);
			}
			opid[opcount] = 10 * opid[opcount] + (int) (*pchar) - 48;
			pchar++;
		}
		if (opid[opcount] >= nchoice) {
			std::cout << "Option id larger than total amount of options in: "
					<< entry << std::endl;
			std::exit(1);
		}
	} else if (nchoice == 1 && pcolon == NULL) {
		while (*pchar != '\0') {
			opvalue_size++;
			opvalue_store[cur++] = *pchar++;
		}
	} else {
		std::cout << "Error: find colon when there is only one choice: "
				<< entry << std::endl;
		std::exit(1);
	}
	opvalue_store[cur] = '\0';
	opvalue_size++;

	opcount++;
}

void Configure::preallocate_memory() {
	opid = new int[max_size];
	opname = new char*[max_size];
	opchoice = new char*[max_size];
	opvalue = new char*[max_size];
	opname_store = new char[max_size];
	opchoice_store = new char[max_size];
	opvalue_store = new char[max_size];
}

void Configure::allocate_memory() {
	opid = new int[opcount];
	opname = new char*[opcount];
	opchoice = new char*[opcount];
	opvalue = new char*[opcount];
	opname_store = new char[opname_size];
	opchoice_store = new char[opchoice_size];
	opvalue_store = new char[opvalue_size];
}

void Configure::clear_memory() {
	delete[] opid;
	delete[] opname;
	delete[] opchoice;
	delete[] opvalue;
	delete[] opname_store;
	delete[] opchoice_store;
	delete[] opvalue_store;
}

void Configure::complete() {

	Configure temp(*this);

	*this = temp;
}

