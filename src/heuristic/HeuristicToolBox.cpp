#include "HeuristicToolBox.h"

void HeuristicToolBox::checkFastDaySeq(Model* m, ConstraintDaysSeq* constraint, int service, int iCons)
{

	for (auto swap : m->getSwapLog()) {
		auto a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent1()];
		auto aIndice = swap.getAgent1();

		if (swap.getService() == service) {
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

				m->getValuation()->mergeDaySeq(v, swap.getDay(), swap.getService(), aIndice, constraint, iCons);

				a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent2()];
				aIndice = swap.getAgent2();
			}
		}

	}
}

void HeuristicToolBox::checkFastInvolved(Model* m, ConstraintInvolved* constraint, int service, int iCons)
{
	bool seqDetected;
	bool found = false;
	int indiceFirst = 0;

	for (auto swap : m->getSwapLog()) {
		auto a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent1()];
		auto aIndice = swap.getAgent1();
		if (swap.getService() == service) {
			for (int j = 0; j < 2; j++) {
				int indice = 0;
				found = false;
				bool first = false;
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

				m->getValuation()->mergeInvolved(v, swap.getDay(), swap.getService(), aIndice, constraint, iCons);

				a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent2()];
				aIndice = swap.getAgent2();
			}
		}
	}
}

void HeuristicToolBox::checkFastSeqMinMax(Model* m, ConstraintSeqMinMax* constraint, int service, int iCons)
{


	for (auto swap : m->getSwapLog()) {


		auto a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent1()];
		auto aIndice = swap.getAgent1();


		if (swap.getService() == service) {
			for (int j = 0; j < 2; j++) {

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

				m->getValuation()->mergeSeqMinMax(v, swap.getDay(), swap.getService(), aIndice, constraint, iCons);

				a = m->getAgentFrom(m->getServices()[swap.getService()])[swap.getAgent2()];
				aIndice = swap.getAgent2();

			}
		}
	}
}

void HeuristicToolBox::checkWorkingHoursWeekFast(Model* m, Agent* agent, int idService, int day, int idA)
{
	auto v = std::vector<std::pair<int, int>>();
	float cptHours = 0;
	int nbFail = 0;
	for (int i = day - 6; i < day + 7; i++) {
		cptHours = 0;

		for (int j = 0; j < 7; j++) {
			if (i + j >= 0 && i + j < 31 && agent->getCalendar()[i + j] != NULL) {
				cptHours += agent->getCalendar()[i + j]->getTime();
			}
		}
		if (cptHours > agent->getNbHoursWeek()) {
			nbFail++;
			v.push_back(std::pair<int, int>(i, i + 6));
		}
	}

	m->getValuation()->mergeHoursWeekSlide(v, day, idService, idA);
}

void HeuristicToolBox::checkImpossiblePostsFast(Model* m, Agent* agent, int idService, int day, int idA)
{
	bool fail = false;
	for (auto ip : agent->getImpossiblePosts()) {
		if (agent->getCalendar()[day] == ip) {
			fail = true;
		}
	}

	m->getValuation()->mergeImpossiblePosts(fail, day, idService, idA);
}

int HeuristicToolBox::getNextDay(int day)
{
	if (day < 0)
		return day;
	return (day + 1) % 7;
}
