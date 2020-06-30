/*
 * Constraint.h
 *
 *  Created on: 10 f�vr. 2020
 *      Author: Valars
 */

enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday, None };

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINT_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINT_H_

class Constraint {

public:

	virtual int getPriority();
};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINT_H_ */
