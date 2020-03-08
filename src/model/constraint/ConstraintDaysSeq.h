/*
 * ConstraintDaysSeq.h
 *
 *  Created on: 10 f�vr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_

#include "Constraint.h"
#include "../Post.h"
#include <vector>

//Correspondant � la contrainte C2

class ConstraintDaysSeq: public Constraint {

public:
	ConstraintDaysSeq(std::vector<Post*> seq, int priority);
	ConstraintDaysSeq(std::vector<string> seq, int priority);
	virtual ~ConstraintDaysSeq();

	int getPriority();
	void setPriority(int priority);

	const std::vector<Post*>& getSequence() const;
	void setSequence(const std::vector<Post*> &sequence);

	const std::vector<string>& getSequenceAtt() const;
	void setSequenceAtt(const std::vector<string> &sequenceAtt);

protected:
	int priority;
	std::vector<Post*> sequence;
	std::vector<string> sequenceAtt;

};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_ */
