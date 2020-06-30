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


enum MinMax {Min, Max};

//Contrainte qui permet d'assurer la présence de la séquence (à partir d'un jour donné) un minimum ou maximum de fois


class ConstraintSeqMinMax: public Constraint {
public:
	ConstraintSeqMinMax(int day, MinMax type, int number, std::vector<Post*> sequence, int priority);
	ConstraintSeqMinMax(int day, MinMax type, int number, std::vector<string> sequence, int priority);
	virtual ~ConstraintSeqMinMax();

	const std::vector<Post*>& getSequence() const;
	void setSequence(const std::vector<Post*> &sequence);

	const std::vector<string>& getSequenceAtt() const;
	void setSequenceAtt(const std::vector<string> &sequenceAtt);

	int getFirstDay() const;
	void setFirstDay(int firstDay);

	int getNumber() const;
	void setNumber(int number);

	MinMax getType() const;
	void setType(MinMax type);

	int getPriority();
	void setPriority(int priority);

	string seqToPrint();

	int check(const Agent* agent, bool checkALL, int firstDayMonth, bool log);
	std::vector<std::pair<int, int>> checkValuation(const Agent* agent, int firstDayMonth);

protected:
	int firstDay;
	MinMax type;
	int number;
	int priority;

	std::vector<Post*> sequence;
	std::vector<string> sequenceAtt;


};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTSEQMINMAX_H_ */
