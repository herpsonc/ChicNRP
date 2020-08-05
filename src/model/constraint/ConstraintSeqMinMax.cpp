/*
 * ConstraintSeqMinMax.cpp
 *
 *  Created on: 8 mars 2020
 *      Author: Valars
 */

#include "ConstraintSeqMinMax.h"


ConstraintSeqMinMax::ConstraintSeqMinMax(int day, MinMax type, int number, std::vector<int> sequence, int priority) {
	this->firstDay=day;
	this->type=type;
	this->number=number;
	this->sequenceAtt=sequence;
	this->priority=priority;

}

ConstraintSeqMinMax::~ConstraintSeqMinMax() {
	// TODO Auto-generated destructor stub
}

const std::vector<int>* ConstraintSeqMinMax::getSequenceAtt() const {
	return &sequenceAtt;
}

void ConstraintSeqMinMax::setSequenceAtt(
		const std::vector<int> &sequenceAtt) {
	this->sequenceAtt = sequenceAtt;
}

int ConstraintSeqMinMax::getFirstDay() const {
	return firstDay;
}

void ConstraintSeqMinMax::setFirstDay(int firstDay) {
	this->firstDay = firstDay;
}

int ConstraintSeqMinMax::getNumber() const {
	return number;
}

void ConstraintSeqMinMax::setNumber(int number) {
	this->number = number;
}

MinMax ConstraintSeqMinMax::getType() const {
	return type;
}

void ConstraintSeqMinMax::setType(MinMax type) {
	this->type = type;
}

const int ConstraintSeqMinMax::getPriority() const{
	return priority;
}

void ConstraintSeqMinMax::setPriority(int priority) {
	this->priority = priority;
}

string ConstraintSeqMinMax::seqToPrint()
{
	string s = "ConstraintSeqMinMax ";
	for (auto att : sequenceAtt) {
		s += att +" ";
	}
	s += " ";
	return s;
}

