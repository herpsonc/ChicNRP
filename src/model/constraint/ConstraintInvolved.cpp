/*
 * ConstraintInvolved.cpp
 *
 *  Created on: 21 févr. 2020
 *      Author: Valars
 */

#include "ConstraintInvolved.h"

using namespace std;

ConstraintInvolved::ConstraintInvolved(vector<Post*> first, vector<Post*> last, int priority) {
	this->firstSeq = first;
	this->lastSeq = last;
	this->priority = priority;

}

ConstraintInvolved::~ConstraintInvolved() {
	// TODO Auto-generated destructor stub
}

const std::vector<Post*>& ConstraintInvolved::getFirstSeq() const {
	return firstSeq;
}

void ConstraintInvolved::setFirstSeq(const std::vector<Post*> &firstSeq) {
	this->firstSeq = firstSeq;
}

const std::vector<Post*>& ConstraintInvolved::getLastSeq() const {
	return lastSeq;
}

void ConstraintInvolved::setLastSeq(const std::vector<Post*> &lastSeq) {
	this->lastSeq = lastSeq;
}

int ConstraintInvolved::getPriority()
{
	return priority;
}

bool ConstraintInvolved::check(const Agent *agent) {
	return false;
}
