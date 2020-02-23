/*
 * ConstraintDayNeed.cpp
 *
 *  Created on: 21 févr. 2020
 *      Author: Valars
 */

#include "ConstraintDayNeed.h"

using namespace std;

ConstraintDayNeed::ConstraintDayNeed(map<Post,int> needs, int priority) {
	this->needs = needs;
	this->priority = priority;

}

ConstraintDayNeed::~ConstraintDayNeed() {
	// TODO Auto-generated destructor stub
}

const std::map<Post, int>& ConstraintDayNeed::getNeeds() const {
	return needs;
}

void ConstraintDayNeed::setNeeds(const std::map<Post, int> &needs) {
	this->needs = needs;
}

int ConstraintDayNeed::getPriority() const {
	return priority;
}

void ConstraintDayNeed::setPriority(int priority) {
	this->priority = priority;
}
