/*
 * ConstraintDaysSeq.cpp
 *
 *  Created on: 10 févr. 2020
 *      Author: Valars
 */

#include "ConstraintDaysSeq.h"

using namespace std;

ConstraintDaysSeq::ConstraintDaysSeq(vector<Post*> seq, int priority) {
	sequence = seq;
	this->priority = priority;

}

ConstraintDaysSeq::ConstraintDaysSeq(vector<string> seq, int priority) {
	sequenceAtt = seq;
	this->priority = priority;

}

ConstraintDaysSeq::~ConstraintDaysSeq() {
	// TODO Auto-generated destructor stub
}

int ConstraintDaysSeq::getPriority()
{
	return priority;
}

void ConstraintDaysSeq::setPriority(int priority) {
	this->priority = priority;
}

const std::vector<Post*>& ConstraintDaysSeq::getSequence() const {
	return sequence;
}

void ConstraintDaysSeq::setSequence(const std::vector<Post*> &sequence) {
	this->sequence = sequence;
}

const std::vector<string>& ConstraintDaysSeq::getSequenceAtt() const {
	return sequenceAtt;
}

void ConstraintDaysSeq::setSequenceAtt(const std::vector<string> &sequenceAtt) {
	this->sequenceAtt = sequenceAtt;
}
