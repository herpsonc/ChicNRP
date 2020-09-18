#include "ConstraintInvolved.h"

using namespace std;

ConstraintInvolved::ConstraintInvolved(vector<int> first,  vector<int> last, int firstDay, int priority) {
	this->firstSeqAtt = first;
	this->lastSeqAtt = last;
	this->priority = priority;
	this->firstDay = firstDay;

}

ConstraintInvolved::~ConstraintInvolved() {

}

const std::vector<int>* ConstraintInvolved::getFirstSeqAtt() const
{
	return &firstSeqAtt;
}

void ConstraintInvolved::setFirstSeqAtt(const std::vector<int>& firstSeq)
{
	this->firstSeqAtt = firstSeq;
}

const std::vector<int>* ConstraintInvolved::getLastSeqAtt() const
{
	return &lastSeqAtt;
}

void ConstraintInvolved::setLastSeqAtt(const std::vector<int>& lastSeq)
{
	this->lastSeqAtt = lastSeq;
}

string ConstraintInvolved::getSeqToPrint()
{
	string s = "ConstraitInvolved ";
	for (auto a : firstSeqAtt) {
		s += a + " ";
	}
	s += "-> ";
	for (auto a : lastSeqAtt) {
		s += a + " ";
	}
	return s;
}

//! \return priority of the constraint
int ConstraintInvolved::getPriority() const
{
	return priority;
}

int ConstraintInvolved::getFirstDay()
{
	return firstDay;
}


