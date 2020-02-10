/*
 * ConstraintDaysSeq.h
 *
 *  Created on: 10 févr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_

#include "Constraint.h"
#include "../Post.h"
#include <vector>

class ConstraintDaysSeq: public Constraint {

public:
	ConstraintDaysSeq(std::vector<Post> seq, int priority);
	virtual ~ConstraintDaysSeq();

	int getPriority();

protected:
	int priority;
	std::vector<Post> sequence;

};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_ */
