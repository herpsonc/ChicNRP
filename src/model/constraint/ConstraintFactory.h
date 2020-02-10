/*
 * ConstraintFactory.h
 *
 *  Created on: 10 févr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTFACTORY_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTFACTORY_H_


#include "ConstraintDaysSeq.h"
#include <vector>

class ConstraintFactory {
public:
	ConstraintFactory();
	virtual ~ConstraintFactory();

	static ConstraintDaysSeq* createConstraintDaysSeq(std::vector<Post>& seq, int priority);
};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTFACTORY_H_ */
