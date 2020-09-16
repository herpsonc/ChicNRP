#include "ConstraintDaysSeq.h"

using namespace std;

ConstraintDaysSeq::ConstraintDaysSeq(vector<int> seq, int priority) {
	sequenceAtt = seq;
	this->priority = priority;

}

ConstraintDaysSeq::~ConstraintDaysSeq() {
}

//! \return priority priority of the constraint
int ConstraintDaysSeq::getPriority() const
{
	return priority;
}

void ConstraintDaysSeq::setPriority(int priority) {
	this->priority = priority;
}

const std::vector<int>* ConstraintDaysSeq::getSequenceAtt() const {
	return &sequenceAtt;
}

void ConstraintDaysSeq::setSequenceAtt(const std::vector<int> &sequenceAtt) {
	this->sequenceAtt = sequenceAtt;
}

string ConstraintDaysSeq::getSeqToPrint() {
	string s = "ConstraintDaySeq ";
	for(auto a : sequenceAtt){
		s+= a+" ";
	}
	return s;
}



