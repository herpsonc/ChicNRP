/*
 * heuristicSolver.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Valars
 */

#include "heuristicSolver.h"


using namespace std;

heuristicSolver::heuristicSolver() {
	// TODO Auto-generated constructor stub

}

heuristicSolver::~heuristicSolver() {
	// TODO Auto-generated destructor stub
}

//Algorithme glouton prenant en compte uniquement les contraintes de postes par jours et d'amplitude horaire sur les agents.
Model heuristicSolver::greedy(const Model m) {
	Model mr = Model(m);

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	auto day = mr.getFirstDay();

	//Pour chaque jours
	int indiceWeek = day; //Indice de la semaine en cours
	for(int i=0;i<mr.getNbDays();i++){

		for(auto s : mr.getServices()){

			//On mélange la liste des agents.
			vector<Agent*> v = mr.getAgentFrom(s);
			shuffle(v.begin(),v.end(),default_random_engine(seed));
			shuffle(v.begin(),v.end(),default_random_engine(seed));

			//On récupère les postes necessaires pour le jour day
			auto required = s->getPostRequired()[day];
			for(auto a : v){

				//Si la journée est affecté à aucun poste
				if(a->getCalendar()[i]==NULL){
					//On regarde les poste restant à attribuer
					for(auto r : required){
						if(r.second>0){
							//On respecte les heures par semaine et par mois
							/*if(a->getWorkingHoursMonth()+r.first->getTime()<=a->getNbHoursMonth()+mr.getOvertime()
									&& a->getWorkingHoursWeek(mr.getFirstDay(),indiceWeek/7)+ r.first->getTime() <= a->getNbHoursWeek()){
								a->setCalendarDay(r.first,i);
								required[r.first] = r.second-1;
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
		day = mr.getNextDay(day);
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

int heuristicSolver::check(Model* m, bool checkALL, bool log) {

	int score = 0;

	bool isValide = true;

	for(auto s : m->getServices()){

		//Check des post requis
		auto day = m->getFirstDay();
		for (int i = 0; i < m->getNbDays(); i++) {
			auto requis = s->getPostRequired()[day];

			for (auto a : m->getAgentFrom(s)) {
				if(requis.find(a->getCalendar()[i]) != requis.end())
					requis[a->getCalendar()[i]]--;
			}

			for (auto r : requis) {
				if (r.second != 0) {
					score--;
					if (log) {
						cout << "Jour " << day << " Post " << r.first->getId() << " insufisant" << endl;
					}
				}

			}
		}

		for(auto a : m->getAgentFrom(s)){
			//Check des heures au mois pour les agents
			if(a->getWorkingHoursMonth() > a->getNbHoursMonth()+m->getOvertime()){
				if(log)
					cout << "Checker: Dépassement d'heure au mois pour l'agent " << a->getId() << endl;
				isValide = false;
				if (checkALL)
					return false;

				score -= 1;
			}
			//Check des heures à la semaine pour les agents
			for(int i=0;i<6;i++){
				if(a->getWorkingHoursWeek(m->getFirstDay(),i) > a->getNbHoursWeek()){
					if (log)
						cout << "Checker: Dépassement d'heure à la semaine " << i << " pour l'agent " << a->getId() << endl;
					isValide = false;
					if(checkALL)
						return false;

					score -= 1;
				}
			}


			for(auto c : s->getConstraints()){
				if(typeid(*c) == typeid(ConstraintDaysSeq)){
						score -= ((ConstraintDaysSeq*)c)->check(a, true, log);
				}
				else if (typeid(*c) == typeid(ConstraintInvolved)) {
						score -= ((ConstraintInvolved*)c)->check(a, true, log);
				}
				else if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
						score -= ((ConstraintSeqMinMax*)c)->check(a, true, m->getFirstDay(), log)*2;
				}
			}
		}
	}


	if(log)
		cout << "Score: " << score << endl;
	return score;
}

Valuation heuristicSolver::checkValuation(Model* m) {

	int score = 0;

	bool isValide = true;

	//Pour Valuation
	int iS = 0;
	int iA = 0;
	int iP = 0;
	auto hoursMonth = vector<vector<int>>();
	auto hoursWeeks = vector<vector<array<int, 6>>>();
	auto daySeq = vector < vector<vector<pair<int, int>>>>();
	auto involved = vector<vector<vector<pair<pair<int, int>, pair<int, int>>>>>();
	auto seqMinMax = vector<vector<vector<pair<int, int>>>>();

	for (auto s : m->getServices()) {

		hoursMonth.push_back(vector<int>());
		hoursWeeks.push_back(vector<array<int, 6>>());
		daySeq.push_back(vector<vector<pair<int,int>>>());
		involved.push_back(vector<vector<pair<pair<int, int>, pair<int, int>>>>());
		seqMinMax.push_back(vector<vector<pair<int, int>>>());

		//Check des post requis
		auto day = m->getFirstDay();
		for (int i = 0; i < m->getNbDays(); i++) {
			auto requis = s->getPostRequired()[day];

			for (auto a : m->getAgentFrom(s)) {
				if (requis.find(a->getCalendar()[i]) != requis.end())
					requis[a->getCalendar()[i]]--;
			}

			for (auto r : requis) {
				if (r.second != 0) {
					score--;
				}

			}
		}


		for (auto a : m->getAgentFrom(s)) {
			//Check des heures au mois pour les agents

			hoursMonth[iS].push_back(a->getWorkingHoursMonth());
			hoursWeeks[iS].push_back(std::array<int, 6>());
			daySeq[iS].push_back(vector<pair<int, int>>());
			involved[iS].push_back(vector<pair<pair<int, int>, pair<int, int>>>());
			seqMinMax[iS].push_back(vector<pair<int, int>>());

			if (a->getWorkingHoursMonth() > a->getNbHoursMonth() + m->getOvertime()) {
				isValide = false;
				
				score -= 1;
			}
			//Check des heures à la semaine pour les agents
			for (int i = 0; i < 6; i++) {

				hoursWeeks[iS][iA][i] = a->getWorkingHoursWeek(m->getFirstDay(), i);

				if (a->getWorkingHoursWeek(m->getFirstDay(), i) > a->getNbHoursWeek()) {
					isValide = false;

					score -= 1;
				}
			}


			for (auto c : s->getConstraints()) {
				if (typeid(*c) == typeid(ConstraintDaysSeq)) {
					daySeq[iS][iA] = ((ConstraintDaysSeq*)c)->checkValuation(a);
					score -= daySeq[iS][iA].size();
				}
				else if (typeid(*c) == typeid(ConstraintInvolved)) {
					involved[iS][iA] = ((ConstraintInvolved*)c)->checkValuation(a);
					score -= involved[iS][iA].size();
				}
				else if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
					seqMinMax[iS][iA]= ((ConstraintSeqMinMax*)c)->checkValuation(a, m->getFirstDay());
					score -= 2*abs((int)(seqMinMax[iS][iA].size() - ((ConstraintSeqMinMax*)c)->getNumber()));
				}
			}
			iA++;
		}
		iS++;
	}


	Valuation v = Valuation();

	v.setScore(score);
	v.setHoursMonth(hoursMonth);
	v.setHoursWeeks(hoursWeeks);
	v.setDaySeq(daySeq);
	v.setInvolved(involved);
	v.setSeqMinMax(seqMinMax);

	return v;
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
		int agent1 = rand() % mr.getAgentFrom(service).size();
		while ((mr.getAgentFrom(service)[agent1]->getCalendarLock()[day] == true) && i < nbIte) {
			agent1 = rand() % mr.getAgentFrom(service).size();
			i++;
			if (i >= nbIte)
				swap = false;
		}

		i = 0;
		int agent2 = rand() % mr.getAgentFrom(service).size();
		while ((mr.getAgentFrom(service)[agent2]->getCalendarLock()[day] == true || agent1 == agent2 ||
			mr.getAgentFrom(service)[agent1]->getCalendar()[day] == mr.getAgentFrom(service)[agent2]->getCalendar()[day]) && i < nbIte) {
			agent2 = rand() % mr.getAgentFrom(service).size();
			i++;
			if (i >= nbIte)
				swap = false;
		}

		//cout << swap << " Swap agent " << mr.getAgentFrom(service)[agent1]->getId() << " et agent " << mr.getAgentFrom(service)[agent2]->getId() << " jour " << day+1 << endl;

		//On swap les poste des deux agents choisit
		if (swap) {
			Post* tmp = mr.getAgentFrom(service)[agent1]->getCalendar()[day];
			mr.getAgentFrom(service)[agent1]->setCalendarDay(mr.getAgentFrom(service)[agent2]->getCalendar()[day], day);
			mr.getAgentFrom(service)[agent2]->setCalendarDay(tmp, day);
		}
	}

	return mr;
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
		bool found = true;
		int agent1 = rand() % mr.getAgentFrom(service).size();
		while ((mr.getAgentFrom(service)[agent1]->getCalendarLock()[day] == true) && i < nbIte) {
			agent1 = rand() % mr.getAgentFrom(service).size();
			i++;
			if (i >= nbIte)
				found = false;
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



Model heuristicSolver::iterative(const Model m, int nbPop, int nbGen, int range)
{
	auto chronoStart = chrono::system_clock::now();

	Model model = greedy(m);
	int bestScore = check(&model, false, false);
	cout << "scoreInit" << bestScore << endl;
	auto pop = vector<Model>();

	for (int j = 0; j < nbGen;j++) {
		cout << "Generation: " << j << endl;
		for (int i = 0;i < nbPop;i++) {
			int randN = rand() % 2;
			switch (randN) {
			case 1:
				pop.push_back(getNeighborSwap(&model, range));
			case 2:
				pop.push_back(getneighborRandom(&model, range));
			}
			
		}

		for (auto e : pop) {
			int res = check(&e, false, false);
			if (res > bestScore) {
				bestScore = res;
				model = e;
			}
		}
		pop.clear();
	}

	auto chronoEnd = chrono::system_clock::now();

	cout << bestScore << " en " << (chronoEnd - chronoStart).count()/10000000 << " secondes" << endl;
	return model;
}

Model heuristicSolver::iterative2(const Model m, int nbIte, int range)
{
	auto chronoStart = chrono::system_clock::now();
	srand(time(0));
	Model currentModel = greedy(m);
	Model bestModel = currentModel;
	Model nextModel = currentModel;
	int bestScore = check(&currentModel, false, false);
	int currentScore = bestScore;
	cout << "scoreInit" << bestScore << endl;

	for (int j = 0; j < nbIte; j++) {
		if (j % 100 == 0) {
			cout << "Iteration " << j << endl;
		}
		//On choisit un voisinnage à appliquer
		int randN = rand() % 100;

		if (randN < 95) {
			nextModel = getNeighborSwap(&currentModel, range);
		}
		else {
			nextModel = getneighborRandom(&currentModel, range);
		}

		//On regarde si la solution est meilleur
		int nextScore = check(&nextModel, false, false);
		if (nextScore > bestScore) {
			bestModel = nextModel;
			bestScore = nextScore;
		}
		//cout << nextScore << " " << bestScore << " " <<currentScore <<endl;
		if (nextScore > currentScore) {
			//80% de chance de choisir le nouveau model
			int randI = rand() % 1000;
			if (randI < 900 ) {
				currentModel = nextModel;
				currentScore = nextScore;
			}
		}
		else {
			//20% de chance de choisir le nouveau candidat même si il est moins bon
			int randI = rand() % 1000;
			if (randI > 998) {
				currentModel = nextModel;
				currentScore = nextScore;
			}
			else {
				int randI = rand() % 1000;
				if (randI > 998) {
					currentModel = greedy(m);
					currentScore = check(&currentModel, false, false);
				}
			}
		}

	}

	auto chronoEnd = chrono::system_clock::now();

	cout << bestScore << " en " << (chronoEnd - chronoStart).count() / 10000000 << " secondes" << endl;

	return bestModel;
}


