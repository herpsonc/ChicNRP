/*
 * ConstraintInvolved.h
 *
 *  Created on: 21 f�vr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_

#include "Constraint.h"
#include "../Agent.h"
#include <iostream>
//Correspondant à la contrainte C6


class ConstraintInvolved: public Constraint {
public:
	ConstraintInvolved(std::vector<string> first, std::vector<string> last, int firstDay, int priority);
	virtual ~ConstraintInvolved();

	const std::vector<string>* getFirstSeqAtt() const;
	void setFirstSeqAtt(const std::vector<string>& firstSeq);
	const std::vector<string>* getLastSeqAtt() const;
	void setLastSeqAtt(const std::vector<string>& lastSeq);

	string getSeqToPrint();

	const int getPriority() const;
	int getFirstDay();

	int check(const Agent* agent, int day, bool log);

protected:
	int priority;
	int firstDay;
	std::vector<string> firstSeqAtt;
	std::vector<string> lastSeqAtt;
};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_ */
