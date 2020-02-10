/*
 * ConstraintDaysSeq.cpp
 *
 *  Created on: 10 févr. 2020
 *      Author: Valars
 */

#include "ConstraintDaysSeq.h"

using namespace std;

ConstraintDaysSeq::ConstraintDaysSeq(vector<Post> seq, int priority) {
	sequence = seq;
	this->priority = priority;

}

ConstraintDaysSeq::~ConstraintDaysSeq() {
	// TODO Auto-generated destructor stub
}

int ConstraintDaysSeq::getPriority()
{
	return priority;
}
