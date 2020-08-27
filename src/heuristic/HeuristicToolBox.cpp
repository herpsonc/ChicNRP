#include "HeuristicToolBox.h"

void HeuristicToolBox::checkFastDaySeq(Model* m, ConstraintDaysSeq* constraint, int service, int iCons)
{

	for (auto swap : *m->getSwapLog()) {
		auto a = (*m->getAgentFromPtr(m->getServices()[swap.getService1()]))[swap.getAgent1()];
		auto aIndice = swap.getAgent1();
		auto idService = swap.getService1();

		for (int j = 0; j < 2; j++) {
			if (idService == service) {
				int indice = 0;
				int iFirst = 0;
				bool found = false;
				auto v = vector<pair<int, int>>();
				//On regarde que les jours nécessaires

				for (int i = swap.getDay() - (int)(*constraint->getSequenceAtt()).size(); i <= swap.getDay() + (int)(*constraint->getSequenceAtt()).size(); i++) {
					if (i >= 0 && i < m->getNbDays()) {
						Post* p = a->getCalendar()[i];
						if (p != NULL) {
							for (int k = 0; k < p->getAttributs().size(); k++) {
								if (!found && p->getAttributs()[k] == (*constraint->getSequenceAtt())[indice]) {
									found = true;
									indice++;
									//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
									if (indice >= (int)(*constraint->getSequenceAtt()).size()) {
										found = false;
										v.push_back(pair<int, int>(i - indice + 1, i));
										indice = 0;
										i = iFirst;
									}
									break;
								}
							}

							if (!found && indice > 0) {
								i = iFirst;
								indice = 0;
							}

							if (indice == 1)
								iFirst = i;

							found = false;
						}
						else {
							indice = 0;
						}
					}
				}

				m->getValuation()->mergeDaySeq(v, swap.getDay(), idService, aIndice, constraint, iCons);

				
			}
			a = (*m->getAgentFromPtr(m->getServices()[swap.getService2()]))[swap.getAgent2()];
			idService = swap.getService2();
			aIndice = swap.getAgent2();
		}
	}
}

void HeuristicToolBox::checkFastInvolved(Model* m, ConstraintInvolved* constraint, int service, int iCons)
{
	bool seqDetected;
	bool found = false;
	int indiceFirst = 0;

	for (auto swap : *m->getSwapLog()) {
		auto a = (*m->getAgentFromPtr(m->getServices()[swap.getService1()]))[swap.getAgent1()];
		auto aIndice = swap.getAgent1();
		auto idService = swap.getService1();
		
		for (int j = 0; j < 2; j++) {
			if (idService == service) {
				int indice = 0;
				found = false;
				bool first = false;
				seqDetected = false;
				auto v = vector<pair<pair<int, int>, pair<int, int>>>();
				int day = m->getFirstDay();

				for (int i = 0; i < swap.getDay() - (int)(*constraint->getFirstSeqAtt()).size() - (int)(*constraint->getLastSeqAtt()).size() + 1; i++) {
					day = getNextDay(day);
				}

				//On regarde que les jours nécessaires
				for (int i = swap.getDay() - (int)(*constraint->getFirstSeqAtt()).size() - (int)(*constraint->getLastSeqAtt()).size() + 1;
					i <= swap.getDay() + (int)(*constraint->getFirstSeqAtt()).size() + (int)(*constraint->getLastSeqAtt()).size(); i++) {
					if (i >= 0 && i < m->getNbDays()) {
						Post* p = a->getCalendar()[i];
						if (indice != 0 || constraint->getFirstDay() == -1 || constraint->getFirstDay() == day || seqDetected) {
							if (p != NULL) {
								for (int k = 0; k < p->getAttributs().size(); k++) {
									//Le cas où la séquence est déjà détectée
									if (!found && seqDetected) {
										if (p->getAttributs()[k] == (*constraint->getLastSeqAtt())[indice]) {
											indice++;
											found = true;

											if (indice == (int)(*constraint->getLastSeqAtt()).size()) {
												seqDetected = false;
												indice = 0;
											}
										}
									}
									//Le cas où la firstSeq n'est pas trouvée
									else {
										if (!found && p->getAttributs()[k] == (*constraint->getFirstSeqAtt())[indice]) {
											indice++;
											found = true;
											//Toute la séquence a été trouvée
											if (indice == (int)(*constraint->getFirstSeqAtt()).size()) {
												indiceFirst = i;
												indice = 0;
												seqDetected = true;
											}
										}
									}
									if (p->getAttributs()[k] == (*constraint->getFirstSeqAtt())[0]) {
										first = true;
									}
								}
							}
						}
						//Si la 2e séquence n'est pas détectée
						if (seqDetected && !found) {
							v.push_back(pair<pair<int, int>, pair<int, int>>(pair<int, int>(indiceFirst - (*constraint->getFirstSeqAtt()).size() + 1, indiceFirst),
								pair<int, int>(indiceFirst + 1, indiceFirst + (*constraint->getLastSeqAtt()).size())));
							// isValide = false;
							seqDetected = false;
							indice = 0;
							if (first) {
								if ((*constraint->getFirstSeqAtt()).size() == 1) {
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

				m->getValuation()->mergeInvolved(v, swap.getDay(), idService, aIndice, constraint, iCons);

				
			}
			a = (*m->getAgentFromPtr(m->getServices()[swap.getService2()]))[swap.getAgent2()];
			idService = swap.getService2();
			aIndice = swap.getAgent2();
		}
	}
}

void HeuristicToolBox::checkFastSeqMinMax(Model* m, ConstraintSeqMinMax* constraint, int service, int iCons)
{


	for (auto swap : *m->getSwapLog()) {


		auto a = (*m->getAgentFromPtr(m->getServices()[swap.getService1()]))[swap.getAgent1()];
		auto aIndice = swap.getAgent1();
		auto idService = swap.getService1();

		for (int j = 0; j < 2; j++) {
			if (idService == service) {

				unsigned int cptCheck = 0;
				unsigned int indice = 0;
				int day = m->getFirstDay();

				for (int i = 0; i < swap.getDay() - (int)(*constraint->getSequenceAtt()).size(); i++) {
					day = getNextDay(day);
				}

				auto v = vector<pair<int, int>>();
				bool start = false;
				bool found = false;

				for (int i = swap.getDay() - (int)(*constraint->getSequenceAtt()).size(); i <= swap.getDay() + (int)(*constraint->getSequenceAtt()).size(); i++) {
					if (i >= 0 && i < m->getNbDays()) {
						Post* p = a->getCalendar()[i];
						if (indice == 0 && constraint->getFirstDay() == day) {
							start = true;
						}

						if (start || indice != 0) {
							for (int k = 0; k < p->getAttributs().size(); k++) {
								if (p->getAttributs()[k] == (*constraint->getSequenceAtt())[indice]) {
									indice++;
									found = true;
									if (indice >= (*constraint->getSequenceAtt()).size()) {
										v.push_back(pair<int, int>(i - (*constraint->getSequenceAtt()).size() + 1, i));
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

				m->getValuation()->mergeSeqMinMax(v, swap.getDay(), idService, aIndice, constraint, iCons);

			}

			a = (*m->getAgentFromPtr(m->getServices()[swap.getService2()]))[swap.getAgent2()];
			aIndice = swap.getAgent2();
			idService = swap.getService2();
		}
	}
}

void HeuristicToolBox::checkWorkingHoursWeekFast(Model* m, Agent* agent, int idService, int day, int idA)
{
	auto v = std::vector<std::pair<int, int>>();
	float cptHours = 0;
	int nbFail = 0;
	for (int i = day - 7; i <= day; i++) {
		cptHours = 0;

		for (int j = 0; j < 7; j++) {
			if (i + j >= 0 && i + j < m->getNbDays() && agent->getCalendar()[i + j] != NULL) {
				cptHours += agent->getCalendar()[i + j]->getTime();
			}
		}
		if (cptHours > agent->getNbHoursWeek()) {
			nbFail++;
			v.push_back(std::pair<int, int>(i, i + 6));
		}
	}

	m->getValuation()->mergeHoursWeekSlide(v, day, idService, idA, agent->getNbHoursWeekPriority());
}

void HeuristicToolBox::checkImpossiblePostsFast(Model* m, Agent* agent, int idService, int day, int idA)
{
	bool fail = false;
	for (auto ip : agent->getImpossiblePosts()) {
		if (agent->getCalendar()[day] == ip) {
			fail = true;
		}
	}

	m->getValuation()->mergeImpossiblePosts(fail, day, idService, idA, agent->getImpossiblePostsPriority());
}

void HeuristicToolBox::checkPostsRequirementFast(Model* m, int idService, int day)
{
	int nbFail = 0;
	auto required = m->getServices()[idService]->getPostRequired()[m->idDayToDay(day)];
	
	for (auto a : *m->getAgentFromPtr(m->getServices()[idService])) {
		auto p = a->getCalendar()[day];
		if (p != NULL && required.find(p) != required.end()) {
			required[p] -= 1;
		}
	}

	for (auto r : required) {
		nbFail += abs(r.second);
	}

	m->getValuation()->mergePostRequirement(idService, day, nbFail, m->getServices()[idService]->getPostRequirementPriority());
}

void HeuristicToolBox::checkAllFast(Model* m)
{
	//On génére un swapLog qui couvre tout les agents/service/jour
	vector<SwapLog> swapLog = vector<SwapLog>();
	int idService = 0;
	int idAgent = 0;
	for (auto service : m->getServices()) {
		idAgent = 0;
		for (auto agent : m->getAgentFrom(service)) {

			for (int day = 0; day < m->getNbDays(); day++) {
				m->addSwapLog(SwapLog(idAgent, idAgent, day, idService, idService, NULL, NULL));

				checkWorkingHoursWeekFast(m, agent, idService, day, idAgent);
				checkImpossiblePostsFast(m, agent, idService, day, idAgent);
			}
			m->getValuation()->mergeHoursMonth(agent->getWorkingHoursMonth(), idService, idAgent, agent->getNbHoursMonth(), agent->getNbHoursMonthPriority());
			idAgent++;
		}

		for (int day = 0; day < m->getNbDays(); day++) {
			checkPostsRequirementFast(m, idService, day);
		}

		int i = 0;
		int j = 0;
		int k = 0;
		for (auto c : service->getConstraints()) {
			if (typeid(*c) == typeid(ConstraintDaysSeq)) {
				HeuristicToolBox::checkFastDaySeq(m, (ConstraintDaysSeq*)c, idService, i);
				i++;
			}
			else if (typeid(*c) == typeid(ConstraintInvolved)) {
				HeuristicToolBox::checkFastInvolved(m, (ConstraintInvolved*)c, idService, j);
				j++;
			}
			else if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
				HeuristicToolBox::checkFastSeqMinMax(m, (ConstraintSeqMinMax*)c, idService, k);
				k++;
			}
		}

		m->resetSwapLog();
		idService++;
	}

	m->resetSwapLog();
}

int HeuristicToolBox::getNextDay(int day)
{
	if (day < 0)
		return day;
	return (day + 1) % 7;
}

void HeuristicToolBox::checkAllDaySeq(Model* m)
{
	int i = 0;
	for (unsigned int l = 0; l < m->getServices().size(); l++) {
		i = 0;
		for (auto c : m->getServices()[l]->getConstraints()) {
			if (typeid(*c) == typeid(ConstraintDaysSeq)) {
				HeuristicToolBox::checkFastDaySeq(m, (ConstraintDaysSeq*)c, l, i);
				i++;
			}
		}
	}
}

void HeuristicToolBox::checkAllInvolved(Model* m)
{
	int j = 0;

	for (unsigned int l = 0; l < m->getServices().size(); l++) {
		j = 0;
		for (auto c : m->getServices()[l]->getConstraints()) {
			if (typeid(*c) == typeid(ConstraintInvolved)) {
				HeuristicToolBox::checkFastInvolved(m, (ConstraintInvolved*)c, l, j);
				j++;
			}
		}
	}
}

void HeuristicToolBox::checkAllSeqMinMax(Model* m)
{
	int k = 0;

	for (unsigned int l = 0; l < m->getServices().size(); l++) {
		k = 0;
		for (auto c : m->getServices()[l]->getConstraints()) {
			if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
				HeuristicToolBox::checkFastSeqMinMax(m, (ConstraintSeqMinMax*)c, l, k);
				k++;
			}
		}
	}
}

