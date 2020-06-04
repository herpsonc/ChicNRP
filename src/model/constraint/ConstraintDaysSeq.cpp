/*
 * ConstraintDaysSeq.cpp
 *
 *  Created on: 10 févr. 2020
 *      Author: Valars
 */

#include "ConstraintDaysSeq.h"

using namespace std;

ConstraintDaysSeq::ConstraintDaysSeq(vector<Post*> seq, int priority) {
	sequence = seq;
	this->priority = priority;

}

ConstraintDaysSeq::ConstraintDaysSeq(vector<string> seq, int priority) {
	sequenceAtt = seq;
	this->priority = priority;

}

ConstraintDaysSeq::~ConstraintDaysSeq() {
}

//! \return priority priority of the constraint

int ConstraintDaysSeq::getPriority()
{
	return priority;
}

void ConstraintDaysSeq::setPriority(int priority) {
	this->priority = priority;
}

const std::vector<Post*>& ConstraintDaysSeq::getSequence() const {
	return sequence;
}

void ConstraintDaysSeq::setSequence(const std::vector<Post*> &sequence) {
	this->sequence = sequence;
}

const std::vector<string>& ConstraintDaysSeq::getSequenceAtt() const {
	return sequenceAtt;
}

void ConstraintDaysSeq::setSequenceAtt(const std::vector<string> &sequenceAtt) {
	this->sequenceAtt = sequenceAtt;
}

string ConstraintDaysSeq::getSeqToPrint() {
	string s = "ConstraintDaySeq ";
	for(auto a : sequenceAtt){
		s+= a+" ";
	}
	return s;
}

//Retourne true si la séquence de la contrainte se retrouve dans le calendrier de l'agent
int ConstraintDaysSeq::check(const Agent *agent, bool checkALL, bool log) {

	unsigned int indice = 0;
	bool found = false;
	int i = 0;
	bool exist = false;
	int nb_fail = 0;
	bool first = false;
	//On prend en compte les 7 jours avant le debut du mois
	for(auto post : agent->getLastMonthCalendar()){
		if(post!=NULL){
			for(auto att : post->getAttributs()){
				if(att==sequenceAtt[indice]){
					found = true;
					indice++;
					//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
					if(indice>=sequenceAtt.size()){
						if(log)
							cout << getSeqToPrint() << ": Agent " << agent->getId() << " Jour "
									<< i-indice+2 << " à " << i+1 << endl;
						exist = true;
						found = false;
						nb_fail++;
						indice = 0;
					}
					break;
				}
				if (att == sequenceAtt[0]) {
					first = true;
				}
			}
			if (!found) {
				indice = 0;
				if (first)
					indice = 1;
			}
			found = false;
			first = false;
		}
		else
			indice=0;
		i++;
	}
	i=0;
	//On continu sur le calendrier
		for(auto post : agent->getCalendar()){
			if(post!=NULL){
				for(auto att : post->getAttributs()){
					if(!found && att==sequenceAtt[indice]){
						found = true;
						indice++;
						//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
						if(indice>=sequenceAtt.size()){
							if(log)
								cout << getSeqToPrint() << ": Agent " << agent->getId() << " Jour "
										<< i-indice+2 << " à " << i+1 << endl;
							exist = true;
							found = false;
							nb_fail++;
							indice = 0;
							if(!checkALL)
								return true;
						}
					}
					if (att == sequenceAtt[0]){
						first = true;
					}
				
				}
				if (!found) {
					indice = 0;
					if (first)
						indice = 1;
				}
				found = false;
				first = false;
			}
			else
				indice=0;
			i++;
		}
	return nb_fail*priority;
}

std::vector<std::pair<int, int>> ConstraintDaysSeq::checkValuation(const Agent* agent) {

	unsigned int indice = 0;
	bool found = false;
	int i = 0;
	bool exist = false;
	int nb_fail = 0;
	bool first = false;
	vector<pair<int, int>> v;

	//On prend en compte les 7 jours avant le debut du mois
	for (auto post : agent->getLastMonthCalendar()) {
		if (post != NULL) {
			for (auto att : post->getAttributs()) {
				if (att == sequenceAtt[indice]) {
					found = true;
					indice++;
					//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
					if (indice >= sequenceAtt.size()) {
						exist = true;
						found = false;
						v.push_back(pair<int,int>(i - indice+1, i));
						nb_fail++;
						indice = 0;
						
					}
					break;
				}
				if (att == sequenceAtt[0]) {
				first = true;
				}
			}
			if (!found) {
				indice = 0;
				if (first)
					indice = 1;
			}
			found = false;
			first = false;
		}
		else
		indice = 0;
		i++;
	}
	i = 0;
	//On continu sur le calendrier
	for (auto post : agent->getCalendar()) {
		if (post != NULL) {
			for (auto att : post->getAttributs()) {
				if (!found && att == sequenceAtt[indice]) {
					found = true;
					indice++;
					//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
					if (indice >= sequenceAtt.size()) {
						exist = true;
						found = false;
						v.push_back(pair<int, int>(i - indice + 1, i));
						nb_fail++;
						indice = 0;
					}
				}
				if (att == sequenceAtt[0]) {
					first = true;
				}
			}
			if (!found) {
				indice = 0;
				if (first)
					indice = 1;
			}
			found = false;
			first = false;
		}
		else
			indice = 0;
		i++;
	}
	return v;
}

void ConstraintDaysSeq::checkFast(Model* m, int iCons)
{
	for (auto swap : m->getSwapLog()) {
		auto a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent1()];
		auto aIndice = swap.getAgent1();


		for (int j = 0; j < 2; j++) {
			int indice = 0;
			bool found = false;
			bool first = false;
			auto v = vector<pair<int, int>>();
			//On regarde que les jours nécessaires

			for (int i = swap.getDay() - (int)this->sequenceAtt.size(); i <= swap.getDay() + (int)this->sequenceAtt.size(); i++) {
				if (i >= 0 && i < m->getNbDays()) {
					Post* p = a->getCalendar()[i];
					if (p != NULL) {
						for (auto att : p->getAttributs()) {
							if (!found && att == sequenceAtt[indice]) {
								found = true;
								indice++;
								//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
								if (indice >= sequenceAtt.size()) {
									found = false;
									v.push_back(pair<int, int>(i - indice + 1, i));
									indice = 0;
								}
							}
							if (att == sequenceAtt[0]) {
								first = true;
							}
						}
						if (!found) {
							indice = 0;
							if (first)
								indice = 1;
						}
						found = false;
						first = false;
					}
					else {
						indice = 0;
					}
				}
			}

			//Update la valuation
			auto valuation = m->getValuation()->getDaySeq()[swap.getService()][aIndice][iCons];
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
					if (value.first == e.first && value.second == e.second) {
						isIn = true;
					}
				}

				if (!isIn) {
					newVec.push_back(e);
					m->getValuation()->setScore(m->getValuation()->getScore() - this->priority);
				}
			}

			auto vecToAdd = m->getValuation()->getDaySeq();
			vecToAdd[swap.getService()][aIndice][iCons] = newVec;
			m->getValuation()->setDaySeq(vecToAdd);

			a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent2()];
			aIndice = swap.getAgent2();
		}

	}
}
