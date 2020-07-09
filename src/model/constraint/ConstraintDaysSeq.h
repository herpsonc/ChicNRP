/*
 * ConstraintDaysSeq.h
 *
 *  Created on: 10 f�vr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_

#include "Constraint.h"
#include "../Agent.h"
#include <iostream>
//Correspondant à la contrainte C2

class ConstraintDaysSeq: public Constraint {

public:
	ConstraintDaysSeq(std::vector<string> seq, int priority);
	virtual ~ConstraintDaysSeq();

	const int getPriority() const;
	void setPriority(int priority);

	const std::vector<string>& getSequenceAtt() const;
	void setSequenceAtt(const std::vector<string> &sequenceAtt);

	string getSeqToPrint();
	int check(const Agent* agent, bool log);
	std::vector<std::pair<int, int>> checkValuation(const Agent* agent);


protected:
	int priority;
	std::vector<string> sequenceAtt;

};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_ */
