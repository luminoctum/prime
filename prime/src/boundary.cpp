/*
 * boundary.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: cli
 */

#include "boundary.h"

void none_0(Variable &var) {
}

void periodic_0(Variable &var) {
	for (int j = 0; j < var.ny; j++)
		for (int i = -var.nh; i < 0; i++)
			var(i, j) = var(i + var.nx, j);
}

void periodic_1(Variable &var) {
	for (int j = 0; j < var.ny; j++)
		for (int i = var.nx; i < var.nx + var.nh; i++)
			var(i, j) = var(i - var.nx, j);
}

void periodic_2(Variable &var) {
	for (int i = 0; i < var.nx; i++)
		for (int j = -var.nh; j < 0; j++)
			var(i, j) = var(i, j + var.ny);
}

void periodic_3(Variable &var) {
	for (int i = 0; i < var.nx; i++)
		for (int j = var.ny; j < var.ny + var.nh; j++)
			var(i, j) = var(i, j - var.ny);
}

void zero_gradient_0(Variable &var) {
	for (int j = 0; j < var.ny; j++)
		for (int i = -var.nh; i < 0; i++)
			var(i, j) = var(0, j);
}

void zero_gradient_1(Variable &var) {
	for (int j = 0; j < var.ny; j++)
		for (int i = var.nx; i < var.nx + var.nh; i++)
			var(i, j) = var(var.nx - 1, j);
}

void zero_gradient_2(Variable &var) {
	for (int i = 0; i < var.nx; i++)
		for (int j = -var.nh; j < 0; j++)
			var(i, j) = var(i, 0);
}

void zero_gradient_3(Variable &var) {
	for (int i = 0; i < var.nx; i++)
		for (int j = var.ny; j < var.ny + var.nh; j++)
			var(i, j) = var(i, var.ny - 1);
}

void fixed_gradient_0(Variable &var){
}

void fixed_gradient_1(Variable &var){
}

void fixed_gradient_2(Variable &var){
}

void fixed_gradient_3(Variable &var){
}

void symmetric_0(Variable &var){
	for (int j = 0; j < var.ny; j++)
		for (int i = -var.nh; i < 0; i++)
			var(i, j) = var(-i - 1, j);
}

void symmetric_1(Variable &var) {
	for (int j = 0; j < var.ny; j++)
		for (int i = var.nx; i < var.nx + var.nh; i++)
			var(i, j) = var(2 * var.nx - i - 1, j);
}

void symmetric_2(Variable &var) {
	for (int i = 0; i < var.nx; i++)
		for (int j = -var.nh; j < 0; j++)
			var(i, j) = var(i, -j - 1);
}

void symmetric_3(Variable &var) {
	for (int i = 0; i < var.nx; i++)
		for (int j = var.ny; j < var.ny + var.nh; j++)
			var(i, j) = var(i, 2 * var.ny - j - 1);
}

void antisymmetric_0(Variable &var){
	for (int j = 0; j < var.ny; j++)
		for (int i = -var.nh; i < 0; i++)
			var(i, j) = -var(-i - 1, j);
}

void antisymmetric_1(Variable &var) {
	for (int j = 0; j < var.ny; j++)
		for (int i = var.nx; i < var.nx + var.nh; i++)
			var(i, j) = -var(2 * var.nx - i - 1, j);
}

void antisymmetric_2(Variable &var) {
	for (int i = 0; i < var.nx; i++)
		for (int j = -var.nh; j < 0; j++)
			var(i, j) = -var(i, -j - 1);
}

void antisymmetric_3(Variable &var) {
	for (int i = 0; i < var.nx; i++)
		for (int j = var.ny; j < var.ny + var.nh; j++)
			var(i, j) = -var(i, 2 * var.ny - j - 1);
}
