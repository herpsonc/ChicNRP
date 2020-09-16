#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_

#include "Constraint.h"
#include "../Agent.h"
#include <iostream>

class ConstraintDaysSeq: public Constraint {

public:
	ConstraintDaysSeq(std::vector<int> seq, int priority);
	virtual ~ConstraintDaysSeq();

	int getPriority() const;
	void setPriority(int priority);

	const std::vector<int>* getSequenceAtt() const;
	void setSequenceAtt(const std::vector<int> &sequenceAtt);

	string getSeqToPrint();

protected:
	int priority;
	std::vector<int> sequenceAtt;

};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_ */
