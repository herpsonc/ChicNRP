/*
 * Agent.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Agent.h"

Agent::Agent() {
	id ="";
	beginner = false;
	inf6Month = false;
	service = NULL;
	nbHours = 0;
}

Agent::~Agent() {
}

bool Agent::isBeginner() const {
	return beginner;
}

void Agent::setBeginner(bool beginner) {
	this->beginner = beginner;
}

const string& Agent::getId() const {
	return id;
}

void Agent::setId(const string &id) {
	this->id = id;
}

bool Agent::isInf6Month() const {
	return inf6Month;
}

void Agent::setInf6Month(bool inf6Month) {
	this->inf6Month = inf6Month;
}

int Agent::getNbHours() const {
	return nbHours;
}

void Agent::setNbHours(int nbHours) {
	this->nbHours = nbHours;
}

