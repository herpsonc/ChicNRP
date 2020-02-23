/*
 * ConstraintTime.h
 *
 *  Created on: 21 févr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTTIME_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTTIME_H_

#include "Constraint.h"

//Correspondant à la contrainte C1

class ConstraintTime: public Constraint {
public:
	ConstraintTime(int time, int priority);
	virtual ~ConstraintTime();
	int getTime() const;
	void setTime(int time);

protected:
	int priority;
	int time;

};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTTIME_H_ */
