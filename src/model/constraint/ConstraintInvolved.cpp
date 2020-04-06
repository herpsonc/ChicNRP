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

ConstraintInvolved::ConstraintInvolved(vector<string> first, vector<string> last, int priority) {
	this->firstSeqAtt = first;
	this->lastSeqAtt = last;
	this->priority = priority;

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

int ConstraintInvolved::check(const Agent *agent, bool checkALL, bool log) {

	unsigned int indice = 0;
	bool seqDetected = false;
	bool found = false;
	bool isValide = true;
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

	i = 0;
	for (auto post : agent->getCalendar()) {
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
					if(log)
						cout << getSeqToPrint() << ": Agent " << agent->getId() << " Jour "
							<< i+1 << endl;
					if (!checkALL)
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
	return nb_fail;
}
