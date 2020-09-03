/*
 * heuristicSolver.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Valars
 */

#include "heuristicSolver.h"
#include <future>


using namespace std;

heuristicSolver::heuristicSolver() {
	// TODO Auto-generated constructor stub

}

heuristicSolver::~heuristicSolver() {
	// TODO Auto-generated destructor stub
}

//Algorithme glouton prenant en compte uniquement les contraintes de postes par jour et d'amplitude horaire sur les agents.
Model heuristicSolver::greedy(const Model m) {
	Model mr = Model(m);


	auto day = mr.getFirstDay();

	//Pour chaque jour
	int indiceWeek = day; //Indice de la semaine en cours
	for(int i=0;i<mr.getNbDays();i++){

		for(auto s : mr.getServices()){

			//On mélange la liste des agents
			vector<Agent*> v = mr.getAgentFrom(s);
			auto seed = chrono::system_clock::now().time_since_epoch().count();
			shuffle(v.begin(),v.end(),default_random_engine(seed));

			//On récupère les postes nécessaires pour le jour day (la variable)
			auto required = s->getPostRequired()[day];


			//On check si certains postes ne sont pas déjà attribués
			for (auto a : v) {
				if (a->getCalendar()[i] != NULL && required.find(a->getCalendar()[i]) != required.end()) {
					required[a->getCalendar()[i]] -= 1;
				}
			}

			for(auto a : v){

				//Si la journée n'est affectée à aucun poste
				if(a->getCalendar()[i]==NULL){
					//On regarde les postes restants à attribuer
					for(auto r : required){
						if(r.second>0){
							//On respecte les heures par semaine et par mois
							/*if(a->getWorkingHoursMonth()+r.first->getTime()<=a->getNbHoursMonth()+mr.getOvertime()
									&& a->getWorkingHoursWeek(mr.getFirstDay(),indiceWeek/7)+ r.first->getTime() <= a->getNbHoursWeek()){
								a->setCalendarDay(r.first,i);
								required[r.first]--; //on enlève un de ces postes de ceux disponibles
								break;
							}*/
							a->setCalendarDay(r.first, i);
							required[r.first] = r.second - 1;
							break;
						}
					}
				}
			}
		}
		day = HeuristicToolBox::getNextDay(day);
		indiceWeek++;
	}
	heuristicSolver::nullTo(&mr,mr.getDefaultPost());
	return mr;
}

void heuristicSolver::nullTo(Model* m, Post* post) {
	for(auto s : m->getServices()){
		for(auto a : m->getAgentFrom(s)){
			for(int i=0;i<m->getNbDays();i++){
				if(a->getCalendar()[i]==NULL){
					a->setCalendarDay(post,i);
				}
			}
		}
	}
}

Model heuristicSolver::removeExtraPosts(const Model* m)
{
	Model mr = Model(*m);

	for (auto service : mr.getServices()) {
		auto day = mr.getFirstDay();

		for (int i = 0; i < m->getNbDays(); i++) {
			//On récupère les postes nécessaires pour le jour day (la variable)
			auto required = service->getPostRequired()[day];


			//On check si certains postes ne sont pas déjà attribués
			for (auto a : mr.getAgentFrom(service)) {
				if (a->getCalendar()[i] != NULL && required.find(a->getCalendar()[i]) != required.end()) {
					required[a->getCalendar()[i]] -= 1;
				}
			}

			for (auto r : required) {
				//Si < 0 alors il y en a trop
				if (r.second < 0) {
					//Pour chaque post en trop
					for (int j = r.second; j < 0; j++) {
						int bestA = 0;
						int bestTime = 0;

						for (int k = 0; k < mr.getAgentFrom(service).size(); k++) {
							if (mr.getAgentFrom(service)[k]->getCalendar()[i] == r.first && mr.getAgentFrom(service)[k]->getWorkingHoursMonth(m->getFirstDay()) > bestTime) {
								bestTime = mr.getAgentFrom(service)[k]->getWorkingHoursMonth(m->getFirstDay());
								bestA = k;
							}
						}
						mr.getAgentFrom(service)[bestA]->setCalendarDay(mr.getDefaultPost(), i);
					}
				}
			}
			day = HeuristicToolBox::getNextDay(day);
		}
	}
	return mr;
}

Model heuristicSolver::predefinedGreedy(const Model m)
{
	Model mr = Model(m);
	int nbFail, nbLock;

	for (auto service : mr.getServices()) {
		auto planning = service->getPredefinedPlanning();

		//Pour chaque ligne du planning, on cherche l'agent le plus contraint
		//(posts impossibles/nombres de posts lock) que l'on peut placer

		vector<bool> locked((*mr.getAgentFromPtr(service)).size(), false );
		for (auto line : *planning->getPlanning()) {
			int bestFail = 1000;
			int bestLock = 0;
			int bestId = 0;
			int i = 0;
			for (auto agent : *mr.getAgentFromPtr(service)) {
				nbFail = 0;
				nbLock = 0;
				if (!locked[i]) {
					for (int i = 0; i < mr.getNbDays(); i++) {
						if (line[i] != NULL) {
							if (agent->getCalendar()[i] != NULL) {
								nbFail++;
								nbLock++;
							}
							else {
								for (auto ip : agent->getImpossiblePosts()) {
									if (line[i] == ip) {
										nbFail++;
										break;
									}
								}
							}
						}
						else {
							if (agent->getCalendar()[i] != NULL) {
								nbLock++;
							}
						}
					}

					//best candidate
					if (nbFail < bestFail) {
						bestId = i;
						bestFail = nbFail;
						bestLock = nbLock;
					}
					else if (nbFail == bestFail && nbLock < bestLock) {
						bestId = i;
						bestFail = nbFail;
						bestLock = nbLock;
					}
				}
				i++;
			}

			(*mr.getAgentFromPtr(service))[bestId]->setCalendarUnlockOnly(line);
			locked[bestId] = true;
		}
	}

	auto mr2 = greedy(removeExtraPosts(&mr));
	return mr2;
}


void heuristicSolver::checkFast(Model* m) {
	auto value = m->getValuation();


	for (auto swap : *m->getSwapLog()) {
		auto agent1 = m->getAgentFrom(m->getServices()[swap.getService1()])[swap.getAgent1()];
		auto agent2 = m->getAgentFrom(m->getServices()[swap.getService2()])[swap.getAgent2()];


		//Check heure au mois
		float dif = swap.getPost1()->getTime() - swap.getPost2()->getTime();
		//cout << dif << " " << swap.getAgent1() << " " << swap.getAgent2() << " " << swap.getDay()<<endl;
		value->mergeHoursMonth(-dif, swap.getService1(), swap.getAgent1(), agent1->getNbHoursMonth(), agent1->getNbHoursMonthPriority());
		value->mergeHoursMonth(dif, swap.getService2(), swap.getAgent2(), agent2->getNbHoursMonth(), agent2->getNbHoursMonthPriority());

		//Check heure semaine
		HeuristicToolBox::checkWorkingHoursWeekFast(m, agent1, swap.getService1(), swap.getDay(), swap.getAgent1());
		HeuristicToolBox::checkWorkingHoursWeekFast(m, agent2, swap.getService2(), swap.getDay(), swap.getAgent2());

		//Check Posts impossibles
		HeuristicToolBox::checkImpossiblePostsFast(m, agent1, swap.getService1(), swap.getDay(), swap.getAgent1());
		HeuristicToolBox::checkImpossiblePostsFast(m, agent2, swap.getService2(), swap.getDay(), swap.getAgent2());

		//Check Posts requirements
		HeuristicToolBox::checkPostsRequirementFast(m, swap.getService1(), swap.getDay());
		if(swap.getService1() != swap.getService2())
			HeuristicToolBox::checkPostsRequirementFast(m, swap.getService2(), swap.getDay());
	}

	//Contraintes
	int i = 0;
	int j = 0;
	int k = 0;

	for (unsigned int l = 0; l < m->getServices().size(); l++) {
		i = 0;
		j = 0;
		k = 0;
		for (auto c : m->getServices()[l]->getConstraints()) {
			if (typeid(*c) == typeid(ConstraintDaysSeq)) {
				HeuristicToolBox::checkFastDaySeq(m, (ConstraintDaysSeq*)c, l, i);
				i++;
			}
			else if (typeid(*c) == typeid(ConstraintInvolved)) {
				HeuristicToolBox::checkFastInvolved(m, (ConstraintInvolved*)c, l, j);
				j++;
			}
			else if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
				HeuristicToolBox::checkFastSeqMinMax(m, (ConstraintSeqMinMax*)c, l, k);
				k++;
			}
		}
	}


}

void heuristicSolver::checkFastMultiThread(Model* m)
{
	auto value = m->getValuation();

	thread t1 = thread(HeuristicToolBox::checkAllDaySeq, m);
	thread t2 = thread(HeuristicToolBox::checkAllInvolved, m);
	thread t3 = thread(HeuristicToolBox::checkAllSeqMinMax, m);

	for (auto swap : *m->getSwapLog()) {
		auto agent1 = m->getAgentFrom(m->getServices()[swap.getService1()])[swap.getAgent1()];
		auto agent2 = m->getAgentFrom(m->getServices()[swap.getService2()])[swap.getAgent2()];


		//Check heure au mois
		float dif = agent1->getCalendar()[swap.getDay()]->getTime() - agent2->getCalendar()[swap.getDay()]->getTime();

		value->mergeHoursMonth(dif, swap.getService1(), swap.getAgent1(), agent1->getNbHoursMonth(), agent1->getNbHoursMonthPriority());
		value->mergeHoursMonth(-dif, swap.getService2(), swap.getAgent2(), agent2->getNbHoursMonth(), agent2->getNbHoursMonthPriority());

		//Check heure semaine
		HeuristicToolBox::checkWorkingHoursWeekFast(m, agent1, swap.getService1(), swap.getDay(), swap.getAgent1());
		HeuristicToolBox::checkWorkingHoursWeekFast(m, agent2, swap.getService2(), swap.getDay(), swap.getAgent2());

		//Check Posts impossibles
		HeuristicToolBox::checkImpossiblePostsFast(m, agent1, swap.getService1(), swap.getDay(), swap.getAgent1());
		HeuristicToolBox::checkImpossiblePostsFast(m, agent2, swap.getService2(), swap.getDay(), swap.getAgent2());

	}

	t1.join();
	t2.join();
	t3.join();
}


Model heuristicSolver::getNeighborSwap(Model* m, int range)
{
	Model mr = Model(*m);

	int nbIte = 100;

	int randRange = (rand() % range) + 1;

	for (int j = 0;j < randRange;j++) {
		//Choix du jour
		int day = rand() % 31;

		//On choisit un service aléatoirement
		int serviceI = rand() % mr.getServices().size();

		Service* service = NULL;
		int i = 0;
		for (auto s : mr.getServices()) {
			if (serviceI == i) {
				service = s;
				break;
			}
			i++;
		}

		bool swap = true;
		i = 0;
		//Choix des deux agents à swap
		int agent1 = rand() % (*mr.getAgentFromPtr(service)).size();
		while (((*mr.getAgentFromPtr(service))[agent1]->getCalendarLock()[day] == true) && i < nbIte) {
			agent1 = rand() % (*mr.getAgentFromPtr(service)).size();
			i++;
			if (i >= nbIte)
				swap = false;
		}

		i = 0;
		int agent2 = rand() % (*mr.getAgentFromPtr(service)).size();
		while (((*mr.getAgentFromPtr(service))[agent2]->getCalendarLock()[day] == true || agent1 == agent2 ||
			(*mr.getAgentFromPtr(service))[agent1]->getCalendar()[day] == (*mr.getAgentFromPtr(service))[agent2]->getCalendar()[day]) && i < nbIte) {
			agent2 = rand() % (*mr.getAgentFromPtr(service)).size();
			i++;
			if (i >= nbIte)
				swap = false;
		}

		//cout << swap << " Swap agent " << mr.getAgentFrom(service)[agent1]->getId() << " et agent " << mr.getAgentFrom(service)[agent2]->getId() << " jour " << day+1 << endl;

		//On swap les postes des deux agents choisis
		if (swap) {
			Post* tmp = (*mr.getAgentFromPtr(service))[agent1]->getCalendar()[day];
			(*mr.getAgentFromPtr(service))[agent1]->setCalendarDay((*mr.getAgentFromPtr(service))[agent2]->getCalendar()[day], day);
			(*mr.getAgentFromPtr(service))[agent2]->setCalendarDay(tmp, day);
			mr.addSwapLog(SwapLog(agent1, agent2, day, serviceI, serviceI, (*mr.getAgentFromPtr(service))[agent2]->getCalendar()[day], (*mr.getAgentFromPtr(service))[agent1]->getCalendar()[day]));
		}
	}

	return mr;
}

void heuristicSolver::neighborSwap(Model* m, int range)
{

	int nbIte = 100;

	int randRange = (rand() % range) + 1;

	for (int j = 0; j < randRange; j++) {
		//Choix du jour
		int day = rand() % 31;

		//On choisit un service aléatoirement
		int serviceI = rand() % m->getServices().size();

		Service* service = NULL;
		int i = 0;
		for (auto s : m->getServices()) {
			if (serviceI == i) {
				service = s;
				break;
			}
			i++;
		}

		bool swap = true;
		i = 0;
		//Choix des deux agents à swap
		int agent1 = rand() % (*m->getAgentFromPtr(service)).size();
		while (((*m->getAgentFromPtr(service))[agent1]->getCalendarLock()[day] == true) && i < nbIte) {
			agent1 = rand() % m->getAgentFrom(service).size();
			i++;
			if (i >= nbIte)
				swap = false;
		}

		i = 0;
		int agent2 = rand() % (*m->getAgentFromPtr(service)).size();
		while (((*m->getAgentFromPtr(service))[agent2]->getCalendarLock()[day] == true || agent1 == agent2 ||
			(*m->getAgentFromPtr(service))[agent1]->getCalendar()[day] == (*m->getAgentFromPtr(service))[agent2]->getCalendar()[day]) && i < nbIte) {
			agent2 = rand() % (*m->getAgentFromPtr(service)).size();
			i++;
			if (i >= nbIte)
				swap = false;
		}

		//cout << swap << " Swap agent " << mr.getAgentFrom(service)[agent1]->getId() << " et agent " << mr.getAgentFrom(service)[agent2]->getId() << " jour " << day+1 << endl;

		//On swap les postes des deux agents choisis
		if (swap) {
			Post* tmp = (*m->getAgentFromPtr(service))[agent1]->getCalendar()[day];
			(*m->getAgentFromPtr(service))[agent1]->setCalendarDay((*m->getAgentFromPtr(service))[agent2]->getCalendar()[day], day);
			(*m->getAgentFromPtr(service))[agent2]->setCalendarDay(tmp, day);
			m->addSwapLog(SwapLog(agent1, agent2, day, serviceI, serviceI, (*m->getAgentFromPtr(service))[agent2]->getCalendar()[day], (*m->getAgentFromPtr(service))[agent1]->getCalendar()[day]));
		}
	}
}

void heuristicSolver::neighborSwapBlock(Model* m, int range)
{
	int nbIte = 100;

	int randRange = (rand() % range) + 1;

	//Choix du jour
	int day = rand() % 31;

	//On choisit un service aléatoirement
	int serviceI = rand() % m->getServices().size();

	Service* service = NULL;
	int i = 0;
	for (auto s : m->getServices()) {
		if (serviceI == i) {
			service = s;
			break;
		}
		i++;
	}

	bool swap = true;
	i = 0;
	//Choix des deux agents à swap
	int agent1 = rand() % (*m->getAgentFromPtr(service)).size();
	while (((*m->getAgentFromPtr(service))[agent1]->getCalendarLock()[day] == true) && i < nbIte) {
		agent1 = rand() % m->getAgentFrom(service).size();
		i++;
		if (i >= nbIte)
			swap = false;
	}

	i = 0;
	int agent2 = rand() % (*m->getAgentFromPtr(service)).size();
	while (((*m->getAgentFromPtr(service))[agent2]->getCalendarLock()[day] == true || agent1 == agent2 ||
		(*m->getAgentFromPtr(service))[agent1]->getCalendar()[day] == (*m->getAgentFromPtr(service))[agent2]->getCalendar()[day]) && i < nbIte) {
		agent2 = rand() % (*m->getAgentFromPtr(service)).size();
		i++;
		if (i >= nbIte)
			swap = false;
	}

	//On cherche un bloc autour du jour
	int blockSize = 0, dayMaxRight= 0, dayMaxLeft = 0;
	bool fail = false;
	int dayI = day;
	//++
	while (blockSize < randRange && !fail) {
		dayI++;
		if (dayI >= m->getNbDays() || ((*m->getAgentFromPtr(service))[agent1]->getCalendarLock()[dayI] ||
			(*m->getAgentFromPtr(service))[agent2]->getCalendarLock()[dayI])) {
			fail = true;
			dayI--;
		}
		else
			blockSize++;
	}
	dayMaxRight = dayI - day;
	fail = false;
	dayI = day;
	//--
	while (blockSize < randRange && !fail) {
		dayI--;
		if ((dayI >= m->getNbDays() || dayI < 0) || ((*m->getAgentFromPtr(service))[agent1]->getCalendarLock()[dayI] ||
			(*m->getAgentFromPtr(service))[agent2]->getCalendarLock()[dayI])) {
			fail = true;
			dayI++;
		}
		else
			blockSize++;
	}
	dayMaxLeft = day - dayI;

	if (swap) {
		for (dayI = day - dayMaxLeft; dayI <= day + dayMaxRight; dayI++) {
			Post* tmp = (*m->getAgentFromPtr(service))[agent1]->getCalendar()[dayI];
			(*m->getAgentFromPtr(service))[agent1]->setCalendarDay((*m->getAgentFromPtr(service))[agent2]->getCalendar()[dayI], dayI);
			(*m->getAgentFromPtr(service))[agent2]->setCalendarDay(tmp, dayI);
			m->addSwapLog(SwapLog(agent1, agent2, dayI, serviceI, serviceI, (*m->getAgentFromPtr(service))[agent2]->getCalendar()[dayI], (*m->getAgentFromPtr(service))[agent1]->getCalendar()[dayI]));
		}
	}
}

void heuristicSolver::neighborSwapPool(Model* m, int range, Service* pool)
{
	int nbIte = 100;

	int randRange = (rand() % range) + 1;

	for (int j = 0; j < randRange; j++) {
		//Choix du jour
		int day = rand() % 31;
		int i = 0;
		int serviceI = 0;
		int servicePool = -1;
		//On choisit un service aléatoirement
		Service* service = NULL;
		while (service == NULL || service == pool) {
			serviceI = rand() % m->getServices().size();

			i = 0;
			for (auto s : m->getServices()) {
				if (serviceI == i) {
					service = s;
				}
				if (s == pool)
					servicePool = i;
				i++;
			}
		}

		bool swap = true;
		i = 0;
		//Choix des deux agents à swap
		int agent1 = rand() % (*m->getAgentFromPtr(service)).size();
		while (((*m->getAgentFromPtr(service))[agent1]->getCalendarLock()[day] == true) && i < nbIte) {
			agent1 = rand() % m->getAgentFrom(service).size();
			i++;
			if (i >= nbIte)
				swap = false;
		}

		i = 0;
		int agent2 = rand() % (*m->getAgentFromPtr(pool)).size();
		while (((*m->getAgentFromPtr(pool))[agent2]->getCalendarLock()[day] == true || (*m->getAgentFromPtr(pool))[agent2]->getCalendar()[day] != m->getDefaultPost() ||
			(*m->getAgentFromPtr(service))[agent1]->getCalendar()[day] == (*m->getAgentFromPtr(pool))[agent2]->getCalendar()[day]) && i < nbIte) {
			agent2 = rand() % (*m->getAgentFromPtr(pool)).size();
			i++;
			if (i >= nbIte)
				swap = false;
		}

		//cout << swap << " Swap agent " << mr.getAgentFrom(service)[agent1]->getId() << " et agent " << mr.getAgentFrom(service)[agent2]->getId() << " jour " << day+1 << endl;

		//On swap les postes des deux agents choisis
		if (swap) {
			Post* tmp = (*m->getAgentFromPtr(service))[agent1]->getCalendar()[day];
			(*m->getAgentFromPtr(service))[agent1]->setCalendarDay((*m->getAgentFromPtr(pool))[agent2]->getCalendar()[day], day);
			(*m->getAgentFromPtr(pool))[agent2]->setCalendarDay(tmp, day);
			m->addSwapLog(SwapLog(agent1, agent2, day, serviceI, servicePool, (*m->getAgentFromPtr(pool))[agent2]->getCalendar()[day], (*m->getAgentFromPtr(service))[agent1]->getCalendar()[day]));
		}
	}
}

Model heuristicSolver::getneighborRandom(Model* m, int range)
{
	Model mr = Model(*m);
	int nbIte = 100;

	int randRange = (rand() % range) + 1;

	for (int j = 0; j < randRange; j++) {
		//Choix du jour
		int day = rand() % 31;

		//On choisit un service aléatoirement
		int serviceI = rand() % mr.getServices().size();

		Service* service = NULL;
		int i = 0;
		for (auto s : mr.getServices()) {
			if (serviceI == i) {
				service = s;
				break;
			}
			i++;
		}

		//Choix des deux agents à swap
		i = 0;
		// bool found = true;
		int agent1 = rand() % mr.getAgentFrom(service).size();
		while ((mr.getAgentFrom(service)[agent1]->getCalendarLock()[day] == true) && i < nbIte) {
			agent1 = rand() % mr.getAgentFrom(service).size();
			i++;
		}

		//Choix d'un nouveau Post
		i = 0;
		Post* post = NULL;
		int PostI = rand() % service->getPosts().size();
		for (auto p : service->getPosts()) {
			if (PostI == i) {
				post = p;
				break;
			}
			i++;
		}

		//On change le post de l'agent
		mr.getAgentFrom(service)[agent1]->setCalendarDay(post, day);


	}
	return mr;
}


Model heuristicSolver::iterativeFast(const Model m, int nbIte, int range, int pool)
{
	auto chronoStart = chrono::system_clock::now();
	srand(time(0));
	//Model currentModel = greedy(m);
	Model currentModel = predefinedGreedy(m);
	currentModel.generateEmptyValuation();
	HeuristicToolBox::checkAllFast(&currentModel);
	Model bestModel = currentModel;
	int bestScore = currentModel.getValuation()->getScore();
	int currentScore = bestScore;
	int bestIte = 0;
	int bestTime = 0;
	cout << "scoreInit" << bestScore << endl;

	for (int j = 0; j < nbIte; j++) {
		if (j % 1000 == 0) {
			cout << "Iteration " << j << endl;
		}
		currentModel.resetSwapLog();

		//Choix du swap
		int randI = rand() % 1000;
		if (randI < 800)
			neighborSwap(&currentModel, range);
		else if (randI > 950 && pool >= 0)
			neighborSwapPool(&currentModel, range, currentModel.getServices()[pool]);
		else
			neighborSwapBlock(&currentModel, range);
			

		//On regarde si la solution est meilleure
		checkFast(&currentModel);
		int nextScore = currentModel.getValuation()->getScore();
		if (nextScore > bestScore) {
			bestModel = currentModel;
			bestScore = nextScore;
			bestIte = j;
			bestTime = (chrono::system_clock::now() - chronoStart).count();
		}
		//cout << nextScore << " " << bestScore << " " <<currentScore <<endl;
		if (nextScore > currentScore) {
			//90% de chance de choisir le nouveau model
			int randI = rand() % 1000;
			if (randI < 900 ) {
				currentScore = nextScore;
			}
			else {
				currentModel.rollBack();
				checkFast(&currentModel);
			}
		}
		else {
			//10% de chance de choisir le nouveau candidat même s'il est moins bon
			int randI = rand() % 1000;
			if (randI > 998) {
				currentScore = nextScore;
			}
			else {
				randI = rand() % 10000;
				if (randI > 9998) {
					cout << "reset" << endl;
					//currentModel = greedy(m);
					currentModel = predefinedGreedy(m);
					currentModel.generateEmptyValuation();
					HeuristicToolBox::checkAllFast(&currentModel);
					currentScore = currentModel.getValuation()->getScore();
				}
				else {
					currentModel.rollBack();
					checkFast(&currentModel);
				}
			}
		}

	}

	auto chronoEnd = chrono::system_clock::now();

	cout << bestScore << " en " << (chronoEnd - chronoStart).count() / 10000000 << " secondes" << endl;
	cout << "best trouve en " << bestIte << " iterations en " << bestTime / 10000000 << " secondes" << endl;

	return bestModel;
}





