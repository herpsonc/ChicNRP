#include "HeuristicToolBox.h"

void HeuristicToolBox::checkFastDaySeq(Model* m, ConstraintDaysSeq* constraint, int iCons)
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

			for (int i = swap.getDay() - (int)constraint->getSequenceAtt().size(); i <= swap.getDay() + (int)constraint->getSequenceAtt().size(); i++) {
				if (i >= 0 && i < m->getNbDays()) {
					Post* p = a->getCalendar()[i];
					if (p != NULL) {
						for (auto att : p->getAttributs()) {
							if (!found && att == constraint->getSequenceAtt()[indice]) {
								found = true;
								indice++;
								//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
								if (indice >= (int)constraint->getSequenceAtt().size()) {
									found = false;
									v.push_back(pair<int, int>(i - indice + 1, i));
									indice = 0;
								}
							}
							if (att == constraint->getSequenceAtt()[0]) {
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
				if (value.first >= swap.getDay() - (int)constraint->getSequenceAtt().size() && value.second <= swap.getDay() + (int)constraint->getSequenceAtt().size()) {
					found = false;
					for (auto e : v) {
						if (value.first == e.first && value.second == e.second) {
							newVec.push_back(value);
							found = true;
						}
					}
					//Si on le trouve pas, c'est qu'on a résolu la contrainte
					if (!found) {
						m->getValuation()->setScore(m->getValuation()->getScore() + constraint->getPriority());
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
					m->getValuation()->setScore(m->getValuation()->getScore() - constraint->getPriority());
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

void HeuristicToolBox::checkFastInvolved(Model* m, ConstraintInvolved* constraint, int iCons)
{
	// bool seqDetected = false;
	bool seqDetected;
	bool found = false;
	// bool isValide = true;
	// int i = 0;
	int indiceFirst = 0;

	for (auto swap : m->getSwapLog()) {
		auto a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent1()];
		auto aIndice = swap.getAgent1();

		for (int j = 0; j < 2; j++) {
			int indice = 0;
			found = false;
			bool first = false;
			// bool seqDetected = false;
			seqDetected = false;
			auto v = vector<pair<pair<int, int>, pair<int, int>>>();
			int day = m->getFirstDay();

			for (int i = 0; i < swap.getDay() - (int)constraint->getFirstSeqAtt().size() - (int)constraint->getLastSeqAtt().size() + 1; i++) {
				day = getNextDay(day);
			}

			//On regarde que les jours nécessaires
			for (int i = swap.getDay() - (int)constraint->getFirstSeqAtt().size() - (int)constraint->getLastSeqAtt().size() + 1; i <= swap.getDay() + (int)constraint->getFirstSeqAtt().size() + (int)constraint->getLastSeqAtt().size(); i++) {
				if (i >= 0 && i < m->getNbDays()) {
					Post* p = a->getCalendar()[i];
					if (indice != 0 || constraint->getFirstDay() == -1 || constraint->getFirstDay() == day || seqDetected) {
						if (p != NULL) {
							for (auto att : p->getAttributs()) {
								//Le cas où la séquence est déjà détectée
								if (!found && seqDetected) {
									if (att == constraint->getLastSeqAtt()[indice]) {
										indice++;
										found = true;

										if (indice == (int)constraint->getLastSeqAtt().size()) {
											seqDetected = false;
											indice = 0;
										}
									}
								}
								//Le cas où la firstSeq n'est pas trouvée
								else {
									if (!found && att == constraint->getFirstSeqAtt()[indice]) {
										indice++;
										found = true;
										//Toute la séquence a été trouvée
										if (indice == (int)constraint->getFirstSeqAtt().size()) {
											indiceFirst = i;
											indice = 0;
											seqDetected = true;
										}
									}
								}
								if (att == constraint->getFirstSeqAtt()[0]) {
									first = true;
								}
							}
						}
					}
					//Si la 2e séquence n'est pas détectée
					if (seqDetected && !found) {
						v.push_back(pair<pair<int, int>, pair<int, int>>(pair<int, int>(indiceFirst - constraint->getFirstSeqAtt().size() + 1, indiceFirst), pair<int, int>(indiceFirst + 1, indiceFirst + constraint->getLastSeqAtt().size())));
						// isValide = false;
						seqDetected = false;
						indice = 0;
						if (first) {
							if (constraint->getFirstSeqAtt().size() == 1) {
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
					day = getNextDay(day);
				}
			}

			//Update la valuation
			auto valuation = m->getValuation()->getInvolved()[swap.getService()][aIndice][iCons];
			auto newVec = vector<pair<pair<int, int>, pair<int, int>>>();
			for (auto value : valuation) {
				//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
				if (value.first.first >= swap.getDay() - (int)constraint->getFirstSeqAtt().size() - (int)constraint->getLastSeqAtt().size() + 1 && value.second.second <= swap.getDay() + (int)constraint->getFirstSeqAtt().size() + (int)constraint->getLastSeqAtt().size() - 1) {
					found = false;
					for (auto e : v) {
						if (value.first.first == e.first.first && value.second.second == e.second.second &&
							value.first.second == e.first.second && value.second.first == e.second.first) {
							newVec.push_back(value);
							found = true;
						}
					}
					//Si on ne la trouve pas, c'est qu'on a résolu la contrainte
					if (!found) {
						m->getValuation()->setScore(m->getValuation()->getScore() + constraint->getPriority());
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
					m->getValuation()->setScore(m->getValuation()->getScore() - constraint->getPriority());
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

void HeuristicToolBox::checkFastSeqMinMax(Model* m, ConstraintSeqMinMax* constraint, int iCons)
{


	for (auto swap : m->getSwapLog()) {


		auto a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent1()];
		auto aIndice = swap.getAgent1();



		for (int j = 0; j < 2; j++) {

			//on récupère le nb de seq detectées auparavant (pour le score)
			int nbSeq = m->getValuation()->getseqMinMax()[swap.getService()][aIndice][iCons].size();

			unsigned int cptCheck = 0;
			unsigned int indice = 0;
			int day = m->getFirstDay();

			for (int i = 0; i < swap.getDay() - (int)constraint->getSequenceAtt().size(); i++) {
				day = getNextDay(day);
			}

			auto v = vector<pair<int, int>>();
			bool start = false;
			bool found = false;

			for (int i = swap.getDay() - (int)constraint->getSequenceAtt().size(); i <= swap.getDay() + (int)constraint->getSequenceAtt().size(); i++) {
				if (i >= 0 && i < m->getNbDays()) {
					Post* p = a->getCalendar()[i];
					if (indice == 0 && constraint->getFirstDay() == day) {
						start = true;
					}

					if (start || indice != 0) {
						for (auto att : p->getAttributs()) {
							if (att == constraint->getSequenceAtt()[indice]) {
								indice++;
								found = true;
								if (indice >= constraint->getSequenceAtt().size()) {
									v.push_back(pair<int, int>(i - constraint->getSequenceAtt().size() + 1, i));
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
					day = getNextDay(day);
				}
			}

			//Update la valuation
			auto valuation = m->getValuation()->getseqMinMax()[swap.getService()][aIndice][iCons];
			auto newVec = vector<pair<int, int>>();
			for (auto value : valuation) {
				//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
				if (value.first >= swap.getDay() - (int)constraint->getSequenceAtt().size() && value.second <= swap.getDay() + (int)constraint->getSequenceAtt().size()) {
					found = false;
					for (auto e : v) {
						if (value.first == e.first && value.second == e.second) {
							newVec.push_back(value);
							found = true;
						}
					}
					// if (!found)
						//cout << "removed" << endl;
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
			if (constraint->getType() == Min) {

				int scoreA = 0;
				int scoreB = 0;
				if (nbSeq < constraint->getNumber())
					scoreA -= (constraint->getNumber() - nbSeq) * constraint->getPriority();
				if ((int)newVec.size() < constraint->getNumber())
					scoreB -= (constraint->getNumber() - newVec.size()) * constraint->getPriority();
				if (scoreA - scoreB != 0)
					m->getValuation()->setScore(m->getValuation()->getScore() - (scoreA - scoreB));
			}
			else {
				int scoreA = 0;
				int scoreB = 0;
				if (nbSeq > constraint->getNumber())
					scoreA -= (nbSeq - constraint->getNumber()) * constraint->getPriority();
				if ((int)newVec.size() > constraint->getNumber())
					scoreB -= (newVec.size() - constraint->getNumber()) * constraint->getPriority();
				m->getValuation()->setScore(m->getValuation()->getScore() - (scoreA - scoreB));
			}

			a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent2()];
			aIndice = swap.getAgent2();

		}
	}
}

int HeuristicToolBox::getNextDay(int day)
{
	if (day < 0)
		return day;
	return (day + 1) % 7;
}
