/*
 * ConstraintInvolved.cpp
 *
 *  Created on: 21 févr. 2020
 *      Author: Valars
 */

#include "ConstraintInvolved.h"

using namespace std;

ConstraintInvolved::ConstraintInvolved(vector<Post*> first, vector<Post*> last, int priority) {
	this->firstSeq = first;
	this->lastSeq = last;
	this->priority = priority;

}

ConstraintInvolved::ConstraintInvolved(vector<string> first,  vector<string> last, Day firstDay, int priority) {
	this->firstSeqAtt = first;
	this->lastSeqAtt = last;
	this->priority = priority;
	this->firstDay = firstDay;

}

ConstraintInvolved::~ConstraintInvolved() {
	// TODO Auto-generated destructor stub
}

const std::vector<Post*>& ConstraintInvolved::getFirstSeq() const {
	return firstSeq;
}

void ConstraintInvolved::setFirstSeq(const std::vector<Post*> &firstSeq) {
	this->firstSeq = firstSeq;
}

const std::vector<Post*>& ConstraintInvolved::getLastSeq() const {
	return lastSeq;
}


void ConstraintInvolved::setLastSeq(const std::vector<Post*> &lastSeq) {
	this->lastSeq = lastSeq;
}

const std::vector<string>& ConstraintInvolved::getFirstSeqAtt() const
{
	return firstSeqAtt;
}

void ConstraintInvolved::setFirstSeqAtt(const std::vector<string>& firstSeq)
{
	this->firstSeqAtt = firstSeq;
}

const std::vector<string>& ConstraintInvolved::getLastSeqAtt() const
{
	return lastSeqAtt;
}

void ConstraintInvolved::setLastSeqAtt(const std::vector<string>& lastSeq)
{
	this->lastSeqAtt = lastSeq;
}

string ConstraintInvolved::getSeqToPrint()
{
	string s = "ConstraitInvolved ";
	for (auto a : firstSeqAtt) {
		s += a + " ";
	}
	s += "-> ";
	for (auto a : lastSeqAtt) {
		s += a + " ";
	}
	return s;
}

int ConstraintInvolved::getPriority()
{
	return priority;
}

const Day ConstraintInvolved::getFirstDay()
{
	return firstDay;
}

int ConstraintInvolved::check(const Agent *agent, Day day, bool checkALL, bool log) {

	unsigned int indice = 0;
	bool seqDetected = false;
	bool found = false;
	bool isValide = true;
	bool first = false;
	int i = 0;
	int nb_fail = 0;
	//On cherche dans le pré planning

	for (auto post : agent->getLastMonthCalendar()) {
		if (post != NULL) {
			for (auto att : post->getAttributs()) {
				found = false;
				//Le cas où la séquence est déjà detecté.
				if (seqDetected) {
					if (att == lastSeqAtt[indice]) {
						indice++;
						found = true;

						if (indice == lastSeqAtt.size()) {
							seqDetected = false;
							indice = 0;
						}
						break;
					}
				}
				//Le cas où la firstSeq est pas trouvé
				else {
					if (att == firstSeqAtt[indice]) {
						indice++;
						found = true;
						//Toute la séquence a été trouver
						if (indice == firstSeqAtt.size()) {
							indice = 0;
							seqDetected = true;
						}
						break;
					}
				}
				//Si la 2e séquence n'est pas détecté
				if (seqDetected && !found) {
					if(!checkALL)
						return false;
					isValide = false;
					nb_fail++;
					seqDetected = false;
					indice = 0;
				}
				//Reset de la première séquence
				else if (!seqDetected && !found) {
					indice = 0;
				}
			}
		}
		i++;
	}

	found = false;
	i = 0;
	for (auto post : agent->getCalendar()) {
		if (indice != 0 || firstDay == Day::None || firstDay == day || seqDetected) {
			if (post != NULL) {
				for (auto att : post->getAttributs()) {
					//Le cas où la séquence est déjà detecté.
					if (!found && seqDetected) {
						if (att == lastSeqAtt[indice]) {
							indice++;
							found = true;

							if (indice == lastSeqAtt.size()) {
								seqDetected = false;
								indice = 0;
							}
						}
					}
					//Le cas où la firstSeq est pas trouvé
					else {
						if (!found && att == firstSeqAtt[indice]) {
							indice++;
							found = true;
							//Toute la séquence a été trouver
							if (indice == firstSeqAtt.size()) {
								indice = 0;
								seqDetected = true;
							}
						}
					}
					if (att == firstSeqAtt[0]) {
						first = true;
					}
				}
			}
		}
		//Si la 2e séquence n'est pas détecté
		if (seqDetected && !found) {
			if (log)
				cout << getSeqToPrint() << ": Agent " << agent->getId() << " Jour "
				<< i + 1 << endl;
			isValide = false;
			nb_fail++;
			seqDetected = false;
			indice = 0;
			if (first) {
				if (firstSeqAtt.size() == 1) {
					seqDetected = true;
				}
				else {
					indice = 1;
				}
			}
		}
		//Reset de la première séquence
		else if (!seqDetected && !found) {
			indice = 0;
			if (first)
				indice = 1;
		}
		first = false;
		found = false;
		i++;
		day = Model::getNextDay(day);
	}
	return nb_fail*priority;
}

std::vector < std::pair<std::pair<int, int>, std::pair<int, int>>> ConstraintInvolved::checkValuation(const Agent* agent, Day day) {

	unsigned int indice = 0;
	bool seqDetected = false;
	bool found = false;
	bool isValide = true;
	bool first = false;
	int i = 0;
	int nb_fail = 0;
	int indiceFirst = 0;

	auto v = std::vector < std::pair<std::pair<int, int>, std::pair<int, int>>>();

	//On cherche dans le pré planning
	for (auto post : agent->getLastMonthCalendar()) {
		if (post != NULL) {
			for (auto att : post->getAttributs()) {
				//Le cas où la séquence est déjà detecté.
				if (!found && seqDetected) {
					if (att == lastSeqAtt[indice]) {
						indice++;
						found = true;

						if (indice >= lastSeqAtt.size()) {
							seqDetected = false;
							indice = 0;
						}
						break;
					}
				}
				//Le cas où la firstSeq est pas trouvé
				else {
					if (!found && att == firstSeqAtt[indice]) {
						indice++;
						found = true;
						//Toute la séquence a été trouver
						if (indice >= firstSeqAtt.size()) {
							indiceFirst = i;
							indice = 0;
							seqDetected = true;
							
						}
						break;
					}
				}
				
			}
		}
		//Si la 2e séquence n'est pas détecté
		if (seqDetected && !found) {
			v.push_back(pair<pair<int, int>, pair<int, int>>(pair<int, int>(indiceFirst - firstSeqAtt.size(), indiceFirst), pair<int, int>(indiceFirst + 1, indiceFirst + 1 + lastSeqAtt.size())));
			isValide = false;
			nb_fail++;
			seqDetected = false;
			indice = 0;
		}
		//Reset de la première séquence
		else if (!seqDetected && !found) {
			indice = 0;
		}
		i++;
		found = false;
	}

	i = 0;
	found = false;
	for (auto post : agent->getCalendar()) {
		if (indice != 0 || firstDay == Day::None || firstDay == day || seqDetected) {
			if (post != NULL) {
				for (auto att : post->getAttributs()) {
					//Le cas où la séquence est déjà detecté.
					if (!found && seqDetected) {
						if (att == lastSeqAtt[indice]) {
							indice++;
							found = true;

							if (indice == lastSeqAtt.size()) {
								seqDetected = false;
								indice = 0;
							}
						}
					}
					//Le cas où la firstSeq est pas trouvé
					else {
						if (!found && att == firstSeqAtt[indice]) {
							indice++;
							found = true;
							//Toute la séquence a été trouver
							if (indice == firstSeqAtt.size()) {
								indiceFirst = i;
								indice = 0;
								seqDetected = true;
							}
						}
					}
					if (att == firstSeqAtt[0]) {
						first = true;
					}
				}
			}
		}
		//Si la 2e séquence n'est pas détecté
		if (seqDetected && !found) {
			v.push_back(pair<pair<int, int>, pair<int, int>>(pair<int, int>(indiceFirst - firstSeqAtt.size() + 1, indiceFirst), pair<int, int>(indiceFirst + 1, indiceFirst + lastSeqAtt.size())));
			isValide = false;
			nb_fail++;
			seqDetected = false;
			indice = 0;
			if (first) {
				if (firstSeqAtt.size() == 1) {
					seqDetected = true;
				}
				else {
					indice = 1;
				}
			}
		}
		//Reset de la première séquence
		else if (!seqDetected && !found) {
			indice = 0;
			if (first)
				indice = 1;
		}
		i++;
		first = false;
		found = false;
		day = Model::getNextDay(day);
	}
	return v;
}

void ConstraintInvolved::checkFast(Model* m, int iCons){
	bool seqDetected = false;
	bool found = false;
	bool isValide = true;
	int i = 0;
	int indiceFirst = 0;

	for (auto swap : m->getSwapLog()) {
		auto a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent1()];
		auto aIndice = swap.getAgent1();

		for (int j = 0; j < 2; j++) {
			int indice = 0;
			bool found = false;
			bool first = false;
			bool seqDetected = false;
			auto v = vector<pair<pair<int, int>, pair<int, int>>>();
			Day day = m->getFirstDay();

			for (int i = 0; i < swap.getDay() - (int)firstSeqAtt.size() - (int)lastSeqAtt.size() + 1; i++) {
				day = Model::getNextDay(day);
			}

			//On regarde que les jours nécessaires
			for (int i = swap.getDay() - (int)firstSeqAtt.size() - (int)lastSeqAtt.size() + 1; i <= swap.getDay() + (int)firstSeqAtt.size() + (int)lastSeqAtt.size(); i++) {
				if (i >= 0 && i < m->getNbDays()) {
					Post* p = a->getCalendar()[i];
					if (indice != 0 || firstDay == Day::None || firstDay == day || seqDetected) {
						if (p != NULL) {
							for (auto att : p->getAttributs()) {
								//Le cas où la séquence est déjà detecté.
								if (!found && seqDetected) {
									if (att == lastSeqAtt[indice]) {
										indice++;
										found = true;

										if (indice == lastSeqAtt.size()) {
											seqDetected = false;
											indice = 0;
										}
									}
								}
								//Le cas où la firstSeq est pas trouvé
								else {
									if (!found && att == firstSeqAtt[indice]) {
										indice++;
										found = true;
										//Toute la séquence a été trouver
										if (indice == firstSeqAtt.size()) {
											indiceFirst = i;
											indice = 0;
											seqDetected = true;
										}
									}
								}
								if (att == firstSeqAtt[0]) {
									first = true;
								}
							}
						}
					}
					//Si la 2e séquence n'est pas détecté
					if (seqDetected && !found) {
						v.push_back(pair<pair<int, int>, pair<int, int>>(pair<int, int>(indiceFirst - firstSeqAtt.size() + 1, indiceFirst), pair<int, int>(indiceFirst + 1, indiceFirst + lastSeqAtt.size())));
						isValide = false;
						seqDetected = false;
						indice = 0;
						if (first) {
							if (firstSeqAtt.size() == 1) {
								seqDetected = true;
							}
							else {
								indice = 1;
							}
						}
					}
					//Reset de la première séquence
					else if (!seqDetected && !found) {
						indice = 0;
						if (first)
							indice = 1;
					}
					first = false;
					found = false;
					day = Model::getNextDay(day);
				}
			}

			//Update la valuation
			auto valuation = m->getValuation()->getInvolved()[swap.getService()][aIndice][iCons];
			auto newVec = vector<pair<pair<int, int>, pair<int, int>>>();
			for (auto value : valuation) {
				//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
				if (value.first.first >= swap.getDay() - (int)firstSeqAtt.size() - (int)lastSeqAtt.size() + 1 && value.second.second <= swap.getDay() + (int)firstSeqAtt.size() + (int)lastSeqAtt.size() - 1) {
					found = false;
					for (auto e : v) {
						if (value.first.first == e.first.first && value.second.second == e.second.second &&
							value.first.second == e.first.second && value.second.first == e.second.first) {
							newVec.push_back(value);
							found = true;
						}
					}
					//Si on le trouve pas, c'est qu'on a résolu la contrainte
					if (!found) {
						m->getValuation()->setScore(m->getValuation()->getScore() + this->priority);
					}
				}
				else {
					newVec.push_back(value);
				}
			}
			//Ajout des nouveaux éléments
			for (auto e : v) {
				bool isIn = false;
				for (auto value : valuation) {
					if (value.first.first == e.first.first && value.second.second == e.second.second &&
						value.first.second == e.first.second && value.second.first == e.second.first) {
						isIn = true;
					}
				}

				if (!isIn) {
					newVec.push_back(e);
					m->getValuation()->setScore(m->getValuation()->getScore() - this->priority);
				}
			}

			auto vecToAdd = m->getValuation()->getInvolved();
			vecToAdd[swap.getService()][aIndice][iCons] = newVec;
			m->getValuation()->setInvolved(vecToAdd);

			a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent2()];
			aIndice = swap.getAgent2();
		}
	}
}
