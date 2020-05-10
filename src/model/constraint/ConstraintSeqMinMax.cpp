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
					//cout << seqToPrint() << " Agent " << agent->getId() << " True " << cptCheck << " found" << endl;
				}
				else
				{
					cout << seqToPrint() << " Agent " << agent->getId() << " False " << cptCheck << " found" << endl;
				}
			}
		
			if (cptCheck >= number)
				return 0;
			return (number - cptCheck)*priority;
		}
		else {
			if (log) {
				if (cptCheck <= number) {
					//cout << seqToPrint() << " Agent " << agent->getId() << " True " << cptCheck << " found" << endl;
				}
				else
				{
					cout << seqToPrint() << " Agent " << agent->getId() << " False" << cptCheck << " found" << endl;
				}
			}

			if (cptCheck <= number)
				return 0;
			return (cptCheck - number)*priority;
		}

		
}

std::vector<std::pair<int, int>> ConstraintSeqMinMax::checkValuation(const Agent* agent, Day firstDayMonth) {
	//TODO
	unsigned int cptCheck = 0;
	unsigned int indice = 0;
	Day day = firstDayMonth;

	bool start = false;
	bool found = false;

	auto v = vector<pair<int, int>>();

	/*
	for (auto post : agent->getLastMonthCalendar()) {

	}*/

	int i = 0;
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
						v.push_back(pair<int, int>(i - sequenceAtt.size() + 1, i));
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
		i++;
	}

	return v;


}

void ConstraintSeqMinMax::checkFast(Model* m, int iCons){


	for (auto swap : m->getSwapLog()) {

		
		auto a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent1()];
		auto aIndice = swap.getAgent1();

		

		for (int j = 0; j < 2; j++) {

			//on récupère le nb de seq detecté auparavant (pour le score)
			int nbSeq = m->getValuation()->getseqMinMax()[swap.getService()][aIndice][iCons].size();

			unsigned int cptCheck = 0;
			unsigned int indice = 0;
			Day day = m->getFirstDay();

			for (int i = 0; i < swap.getDay() - (int)sequenceAtt.size(); i++) {
				day = Model::getNextDay(day);
			}

			auto v = vector<pair<int, int>>();
			bool start = false;
			bool found = false;

			for (int i = swap.getDay() - (int)this->sequenceAtt.size(); i <= swap.getDay() + (int)this->sequenceAtt.size(); i++) {
				if (i >= 0 && i < m->getNbDays()) {
					Post* p = a->getCalendar()[i];
					if (indice == 0 && firstDay == day) {
						start = true;
					}

					if (start || indice != 0) {
						for (auto att : p->getAttributs()) {
							if (att == sequenceAtt[indice]) {
								indice++;
								found = true;
								if (indice >= sequenceAtt.size()) {
									v.push_back(pair<int, int>(i - sequenceAtt.size() + 1, i));
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
			}

			//Update la valuation
			auto valuation = m->getValuation()->getseqMinMax()[swap.getService()][aIndice][iCons];
			auto newVec = vector<pair<int, int>>();
			for (auto value : valuation) {
				//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
				if (value.first >= swap.getDay() - (int)this->sequenceAtt.size() && value.second <= swap.getDay() + (int)this->sequenceAtt.size()) {
					found = false;
					for (auto e : v) {
						if (value.first == e.first && value.second == e.second) {
							newVec.push_back(value);
							found = true;
						}
					}
					if (!found)
						;//cout << "removed" << endl;
				}
				else {
					newVec.push_back(value);
				}
			}
			//Ajout des nouveaux éléments
			for (auto e : v) {
				bool isIn = false;
				for (auto value : valuation) {
					if (value.first == e.first && value.second == e.second) {
						isIn = true;
					}
				}

				if (!isIn) {
					newVec.push_back(e);
				}
			}

			auto vecToAdd = m->getValuation()->getseqMinMax();
			vecToAdd[swap.getService()][aIndice][iCons] = newVec;
			m->getValuation()->setSeqMinMax(vecToAdd);

			//Ajuste le score
			if (type == Min) {

				int scoreA = 0;
				int scoreB = 0;
				if(nbSeq < number)
					scoreA -= (number - nbSeq)* priority;
				if (newVec.size() < number)
					scoreB -= (number - newVec.size()) * priority;
				if (scoreA - scoreB != 0)
				m->getValuation()->setScore(m->getValuation()->getScore() - (scoreA - scoreB));
			}
			else {
				int scoreA = 0;
				int scoreB = 0;
				if (nbSeq > number)
					scoreA -= (nbSeq - number) * priority;
				if (newVec.size() > number)
					scoreB -= (newVec.size() - number) * priority;
				m->getValuation()->setScore(m->getValuation()->getScore() - (scoreA - scoreB));
			}

			a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent2()];
			aIndice = swap.getAgent2();

		}
	}
}

