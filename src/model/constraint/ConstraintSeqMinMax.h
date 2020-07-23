/*
 * ConstraintSeqMinMax.h
 *
 *  Created on: 8 mars 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTSEQMINMAX_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTSEQMINMAX_H_

#include "Constraint.h"
#include "../Agent.h"
#include <iostream>

enum MinMax {Min, Max};

//Contrainte qui permet d'assurer la présence de la séquence (à partir d'un jour donné) un minimum ou maximum de fois


class ConstraintSeqMinMax: public Constraint {
public:
	ConstraintSeqMinMax(int day, MinMax type, int number, std::vector<string> sequence, int priority);
	virtual ~ConstraintSeqMinMax();

	const std::vector<string>* getSequenceAtt() const;
	void setSequenceAtt(const std::vector<string> &sequenceAtt);

	int getFirstDay() const;
	void setFirstDay(int firstDay);

	int getNumber() const;
	void setNumber(int number);

	MinMax getType() const;
	void setType(MinMax type);

	const int getPriority() const;
	void setPriority(int priority);

	string seqToPrint();

	int check(const Agent* agent, int firstDayMonth, bool log);

protected:
	int firstDay;
	MinMax type;
	int number;
	int priority;

	std::vector<string> sequenceAtt;


};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTSEQMINMAX_H_ */
