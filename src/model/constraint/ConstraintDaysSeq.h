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

//Correspondant à la contrainte C2

class ConstraintDaysSeq: public Constraint {

public:
	ConstraintDaysSeq(std::vector<Post> seq, int priority);
	virtual ~ConstraintDaysSeq();

	int getPriority();
	void setPriority(int priority);
	const std::vector<Post>& getSequence() const;
	void setSequence(const std::vector<Post> &sequence);

protected:
	int priority;
	std::vector<Post> sequence;

};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTDAYSSEQ_H_ */
