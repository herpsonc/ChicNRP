/*
 * ConstraintTime.cpp
 *
 *  Created on: 21 févr. 2020
 *      Author: Valars
 */

#include "ConstraintTime.h"

ConstraintTime::ConstraintTime(int time, int priority) {
	this->time = time;
	this->priority = priority;

}

ConstraintTime::~ConstraintTime() {
	// TODO Auto-generated destructor stub
}

int ConstraintTime::getTime() const {
	return time;
}

void ConstraintTime::setTime(int time) {
	this->time = time;
}
