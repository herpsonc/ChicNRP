#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTSEQMINMAX_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTSEQMINMAX_H_

#include "Constraint.h"
#include "../Agent.h"
#include <iostream>

enum MinMax {Min, Max};

/*! \class ConstraintSeqMinMax
 * 	This class contain informations about the constraint C7 (check report for more)
 */

class ConstraintSeqMinMax: public Constraint {
public:
	ConstraintSeqMinMax(int day, MinMax type, int number, std::vector<int> sequence, int priority);
	virtual ~ConstraintSeqMinMax();

	const std::vector<int>* getSequenceAtt() const;
	void setSequenceAtt(const std::vector<int> &sequenceAtt);

	int getFirstDay() const;
	void setFirstDay(int firstDay);

	int getNumber() const;
	void setNumber(int number);

	MinMax getType() const;
	void setType(MinMax type);

	int getPriority() const;
	void setPriority(int priority);

	string seqToPrint();

protected:
	int firstDay;
	MinMax type;
	int number;
	int priority;

	std::vector<int> sequenceAtt;


};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTSEQMINMAX_H_ */
