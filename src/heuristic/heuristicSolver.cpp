/*
 * heuristicSolver.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Valars
 */

#include "heuristicSolver.h"

#include "../model/constraint/ConstraintDaysSeq.h"
#include "../model/constraint/ConstraintInvolved.h"
#include "../model/constraint/ConstraintSeqMinMax.h"
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
							if(a->getWorkingHoursMonth()+r.first->getTime()<=a->getNbHoursMonth()+mr.getOvertime()
									&& a->getWorkingHoursWeek(mr.getFirstDay(),indiceWeek/7)+ r.first->getTime() <= a->getNbHoursWeek()){
								a->setCalendarDay(r.first,i);
								required[r.first] = r.second-1;
								break;
							}
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

bool heuristicSolver::check(Model* m, bool checkALL) {

	bool isValide = true;

	for(auto s : m->getServices()){
		for(auto a : m->getAgentFrom(s)){
			//Check des heures au mois pour les agents
			if(a->getWorkingHoursMonth() > a->getNbHoursMonth()+m->getOvertime()){
				cout << "Checker: Dépassement d'heure au mois pour l'agent " << a->getId() << endl;
				return false;
			}
			//Check des heures à la semaine pour les agents
			for(int i=0;i<6;i++){
				if(a->getWorkingHoursWeek(m->getFirstDay(),i) > a->getNbHoursWeek()){
					cout << "Checker: Dépassement d'heure à la semaine " << i << " pour l'agent " << a->getId() << endl;
					isValide = false;
					if(checkALL)
						return false;
				}
			}

			for(auto c : s->getConstraints()){
				if(typeid(*c) == typeid(ConstraintDaysSeq)){

					if(((ConstraintDaysSeq*)c)->check(a,true)){
						isValide = false;
						if (checkALL)
							return false;
					}

				}
				else if (typeid(*c) == typeid(ConstraintInvolved)) {
					if (((ConstraintInvolved*)c)->check(a, true)) {
						isValide = false;
						if (checkALL)
							return false;
					}
				}
				else if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
					if (((ConstraintSeqMinMax*)c)->check(a, true, m->getFirstDay())) {
						isValide = false;
						if (checkALL)
							return false;
					}
				}
			}
		}
	}
	return isValide;
}
