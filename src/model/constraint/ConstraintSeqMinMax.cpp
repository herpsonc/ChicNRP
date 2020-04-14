/*
 * ConstraintSeqMinMax.cpp
 *
 *  Created on: 8 mars 2020
 *      Author: Valars
 */

#include "ConstraintSeqMinMax.h"

ConstraintSeqMinMax::ConstraintSeqMinMax(Day day, MinMax type, int number, std::vector<Post*> sequence, int priority) {
	this->firstDay=day;
	this->type=type;
	this->number=number;
	this->sequence=sequence;
	this->priority=priority;

}

ConstraintSeqMinMax::ConstraintSeqMinMax(Day day, MinMax type, int number, std::vector<string> sequence, int priority) {
	this->firstDay=day;
	this->type=type;
	this->number=number;
	this->sequenceAtt=sequence;
	this->priority=priority;

}

ConstraintSeqMinMax::~ConstraintSeqMinMax() {
	// TODO Auto-generated destructor stub
}

const std::vector<Post*>& ConstraintSeqMinMax::getSequence() const {
	return sequence;
}

void ConstraintSeqMinMax::setSequence(const std::vector<Post*> &sequence) {
	this->sequence = sequence;
}

const std::vector<string>& ConstraintSeqMinMax::getSequenceAtt() const {
	return sequenceAtt;
}

void ConstraintSeqMinMax::setSequenceAtt(
		const std::vector<string> &sequenceAtt) {
	this->sequenceAtt = sequenceAtt;
}

Day ConstraintSeqMinMax::getFirstDay() const {
	return firstDay;
}

void ConstraintSeqMinMax::setFirstDay(Day firstDay) {
	this->firstDay = firstDay;
}

int ConstraintSeqMinMax::getNumber() const {
	return number;
}

void ConstraintSeqMinMax::setNumber(int number) {
	this->number = number;
}

MinMax ConstraintSeqMinMax::getType() const {
	return type;
}

void ConstraintSeqMinMax::setType(MinMax type) {
	this->type = type;
}

int ConstraintSeqMinMax::getPriority(){
	return priority;
}

void ConstraintSeqMinMax::setPriority(int priority) {
	this->priority = priority;
}

string ConstraintSeqMinMax::seqToPrint()
{
	string s = "ConstraintSeqMinMax ";
	for (auto att : sequenceAtt) {
		s += att +" ";
	}
	s += " ";
	return s;
}

int ConstraintSeqMinMax::check(const Agent* agent, bool checkALL, Day firstDayMonth, bool log) {
	//TODO
	unsigned int cptCheck = 0;
	unsigned int indice = 0;
	Day day = firstDayMonth;

	bool start = false;
	bool found = false;

	/*
	for (auto post : agent->getLastMonthCalendar()) {

	}*/


	for (auto post : agent->getCalendar()) {
		if (indice == 0 && firstDay == day) {
			start = true;
		}

		if (start || indice != 0) {
			for (auto att : post->getAttributs()) {
				if (att == sequenceAtt[indice]) {
					indice++;
					found = true;
					if (indice >= sequenceAtt.size()) {
						cptCheck++;
						indice = 0;
					}
					break;
				}
			}
			if (found == false)
			{
				indice = 0;
			}
			start = false;
		}
		found = false;
		day = Model::getNextDay(day);
	}
	
		if (type == Min) {
			if (log) {
				if (cptCheck >= number) {
					cout << seqToPrint() << " Agent " << agent->getId() << " True " << cptCheck << " found" << endl;
				}
				else
				{
					cout << seqToPrint() << " Agent " << agent->getId() << " False " << cptCheck << " found" << endl;
				}
			}
		
			if (cptCheck >= number)
				return 0;
			return number - cptCheck;
		}
		else {
			if (log) {
				if (cptCheck <= number) {
					cout << seqToPrint() << " Agent " << agent->getId() << " True " << cptCheck << " found" << endl;
				}
				else
				{
					cout << seqToPrint() << " Agent " << agent->getId() << " False" << cptCheck << " found" << endl;
				}
			}

			if (cptCheck <= number)
				return 0;
			return cptCheck - number;
		}

		
}