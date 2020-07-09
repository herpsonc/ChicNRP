/*
 * ConstraintInvolved.cpp
 *
 *  Created on: 21 f�vr. 2020
 *      Author: Valars
 */

#include "ConstraintInvolved.h"

using namespace std;

ConstraintInvolved::ConstraintInvolved(vector<string> first,  vector<string> last, int firstDay, int priority) {
	this->firstSeqAtt = first;
	this->lastSeqAtt = last;
	this->priority = priority;
	this->firstDay = firstDay;

}

ConstraintInvolved::~ConstraintInvolved() {
	// TODO Auto-generated destructor stub
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

const int ConstraintInvolved::getPriority() const
{
	return priority;
}

int ConstraintInvolved::getFirstDay()
{
	return firstDay;
}

int ConstraintInvolved::check(const Agent *agent, int day, bool log) {

	unsigned int indice = 0;
	bool seqDetected = false;
	bool found = false;
	// bool isValide = true;
	bool first = false;
	int i = 0;
	int nb_fail = 0;
	//Recherche dans le pré-planning

	for (auto post : agent->getLastMonthCalendar()) {
		if (post != NULL) {
			for (auto att : post->getAttributs()) {
				found = false;
				//Si la séquence est déjà detectée
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
				//Si la firstSeq n'est pas trouvée
				else {
					if (att == firstSeqAtt[indice]) {
						indice++;
						found = true;
						//Toute la séquence a été trouvée
						if (indice == firstSeqAtt.size()) {
							indice = 0;
							seqDetected = true;
						}
						break;
					}
				}
				//Si la 2e séquence n'est pas détectée
				if (seqDetected && !found) {
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
		// Trouver une alternative au None (ici, on a mis -1)
		if (indice != 0 || firstDay == -1 || firstDay == day || seqDetected) {
			if (post != NULL) {
				for (auto att : post->getAttributs()) {
					//Si la séquence est déjà detectée
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
					//Si la firstSeq n'est pas trouvée
					else {
						if (!found && att == firstSeqAtt[indice]) {
							indice++;
							found = true;
							//Toute la séquence a été trouvée
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
		//Si la 2e séquence n'est pas détectée
		if (seqDetected && !found) {
			if (log)
				cout << getSeqToPrint() << ": Agent " << agent->getId() << " Jour "
				<< i + 1 << endl;
			// isValide = false;
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
		day = (day+1)%7;
	}
	return nb_fail*priority;
}

std::vector < std::pair<std::pair<int, int>, std::pair<int, int>>> ConstraintInvolved::checkValuation(const Agent* agent, int day) {

	unsigned int indice = 0;
	bool seqDetected = false;
	bool found = false;
	// bool isValide = true;
	bool first = false;
	int i = 0;
	int nb_fail = 0;
	int indiceFirst = 0;

	auto v = std::vector < std::pair<std::pair<int, int>, std::pair<int, int>>>();

	//Recherche dans le pré-planning
	for (auto post : agent->getLastMonthCalendar()) {
		if (post != NULL) {
			for (auto att : post->getAttributs()) {
				//Si la séquence est déjà détectée
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
				//Si la firstSeq n'est pas trouvée
				else {
					if (!found && att == firstSeqAtt[indice]) {
						indice++;
						found = true;
						//Toute la séquence a été trouvée
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
		//Si la 2e séquence n'est pas détectée
		if (seqDetected && !found) {
			v.push_back(pair<pair<int, int>, pair<int, int>>(pair<int, int>(indiceFirst - firstSeqAtt.size(), indiceFirst), pair<int, int>(indiceFirst + 1, indiceFirst + 1 + lastSeqAtt.size())));
			// isValide = false;
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
		// Trouver une alternative au None (ici, on a mis -1)
		if (indice != 0 || firstDay == -1 || firstDay == day || seqDetected) {
			if (post != NULL) {
				for (auto att : post->getAttributs()) {
					//Si la séquence est déjà détectée
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
					//Si la firstSeq n'est pas trouvée
					else {
						if (!found && att == firstSeqAtt[indice]) {
							indice++;
							found = true;
							//Toute la séquence a été trouvée
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
		//Si la 2e séquence n'est pas détectée
		if (seqDetected && !found) {
			v.push_back(pair<pair<int, int>, pair<int, int>>(pair<int, int>(indiceFirst - firstSeqAtt.size() + 1, indiceFirst), pair<int, int>(indiceFirst + 1, indiceFirst + lastSeqAtt.size())));
			// isValide = false;
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
		day = (day+1)%7;
	}
	return v;
}

