/*
 * helper.cpp
 *
 *  Created on: Jan 4, 2015
 *      Author: cli
 */

#include "helper.h"

void tridiag_solver(long n, FLOAT *a, FLOAT *b, FLOAT *c, FLOAT *r, FLOAT *v) {
    /* Purpose: Solve tri-diagnol matrix
     * Requirement: a,b,c are the sub-diag, main-diag, sub-diag, r is right hand side, v is result
     * Reference:   wiki, tri-diagnol matrix */
    long i; FLOAT m;
    for (i=1; i<n; i++){
        m=a[i-1]/b[i-1];
        b[i]=b[i]-m*c[i-1];
        r[i]=r[i]-m*r[i-1];
    };
    v[n-1]=r[n-1]/b[n-1];
    for (i=n-2; i>=0; i--)
        v[i]=(r[i]-c[i]*v[i+1])/b[i];
}
