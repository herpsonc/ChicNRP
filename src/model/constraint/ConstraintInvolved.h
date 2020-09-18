#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_

#include "Constraint.h"
#include "../Agent.h"
#include <iostream>

/*! \class ConstraintInvolved
 * 	This class contain informations about the constraint C6 (check report for more)
 */

class ConstraintInvolved: public Constraint {
public:
	ConstraintInvolved(std::vector<int> first, std::vector<int> last, int firstDay, int priority);
	virtual ~ConstraintInvolved();

	const std::vector<int>* getFirstSeqAtt() const;
	void setFirstSeqAtt(const std::vector<int>& firstSeq);
	const std::vector<int>* getLastSeqAtt() const;
	void setLastSeqAtt(const std::vector<int>& lastSeq);

	string getSeqToPrint();

	int getPriority() const;
	int getFirstDay();

protected:
	int priority;
	int firstDay;
	std::vector<int> firstSeqAtt;
	std::vector<int> lastSeqAtt;
};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_ */
