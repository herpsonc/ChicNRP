//============================================================================
// Name        : PANDROIDE.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>

#include "heuristic/heuristicSolver.h"
#include "model/Agent.h"
#include "model/constraint/Constraint.h"
#include "model/constraint/ConstraintDaysSeq.h"
#include "model/Model.h"
#include "model/constraint/ConstraintInvolved.h"
#include "model/constraint/ConstraintSeqMinMax.h"
using namespace std;

Model generateGhr() {

		//Creation du model pour le service GHR
		Model m = Model(Day::Sunday, 31, 25);

		Service* ghr = new Service("GHR");

		m.addService(ghr);

		//Posts
		Post* jg = new Post("Jg", 12.25);
		jg->addAttribut("workL");
		jg->addAttribut("work");
		jg->addAttribut("day");
		Post* ng = new Post("Ng", 12.25);
		ng->addAttribut("workL");
		ng->addAttribut("work");
		ng->addAttribut("night");
		Post* mat = new Post("Mat", 7.5);
		mat->addAttribut("work");
		mat->addAttribut("day");
		Post* repos = new Post("Repos", 0.0);
		repos->addAttribut("rest");
		Post* ca = new Post("Ca", 0.0);
		ca->addAttribut("rest");
		ca->addAttribut("ca");

		Post* oe = new Post("O/E", 7.5);
		oe->addAttribut("work");
		oe->addAttribut("day");

		Post* fp = new Post("FP", 7.5);
		fp->addAttribut("work");
		fp->addAttribut("day");

		Post* cs = new Post("CS", 7.5);
		cs->addAttribut("work");
		cs->addAttribut("day");

		m.setDefaultPost(repos);

		ghr->addPost(jg);
		ghr->addPost(ng);
		ghr->addPost(mat);
		ghr->addPost(repos);
		//ghr->addPost(ca);

		ghr->addPostRequired(jg, 1);
		ghr->addPostRequired(mat, 1);
		ghr->addPostRequired(ng, 1);


		//Contraintes

		//Pas de Nuit/Jour
		auto v = vector<string>();
		v.push_back("night");
		v.push_back("day");
		ConstraintDaysSeq* cJN = new ConstraintDaysSeq(v, 1);

		//Pas 3 jours/nuit de travail d'affilé
		v = vector<string>();
		v.push_back("workL");
		v.push_back("workL");
		v.push_back("workL");
		ConstraintDaysSeq* c3N = new ConstraintDaysSeq(v, 1);

		//Pas de nuit avant un congé posé
		v = vector<string>();
		v.push_back("night");
		v.push_back("ca");
		ConstraintDaysSeq* crn = new ConstraintDaysSeq(v, 1);

		//Après 2 jours/nuits au moins 2 repos
		v = vector<string>();
		v.push_back("workL");
		v.push_back("workL");
		auto v2 = vector<string>();
		v2.push_back("rest");
		v2.push_back("rest");
		ConstraintInvolved* cnr = new ConstraintInvolved(v, v2, 1);

		//1 week ends par mois
		v = vector<string>();
		v.push_back("work");
		v.push_back("work");
		ConstraintSeqMinMax* cwe = new ConstraintSeqMinMax(Day::Saturday,MinMax::Min,1,v,1);

		ghr->addConstraint(cJN);
		ghr->addConstraint(c3N);
		ghr->addConstraint(crn);
		ghr->addConstraint(cnr);
		ghr->addConstraint(cwe);


		float nbHoursWeek = 48.0;

		//Agents
		Agent* a1 = new Agent("1",155, nbHoursWeek,  Status::Confirmed);
		a1->setCalendarDay(oe,3, true);
		a1->setCalendarDay(oe,10, true);
		a1->setCalendarDay(oe,17, true);
		a1->setCalendarDay(oe,24, true);
		a1->setCalendarDay(ca,0, true);
		a1->setCalendarDay(ca,1, true);
		a1->setCalendarDay(ca,2, true);
		a1->setCalendarDay(ca,25, true);
		a1->setCalendarDay(ca,26, true);
		a1->setCalendarDay(ca,27, true);
		a1->setCalendarDay(ca,28, true);
		a1->setCalendarDay(ca,29, true);
		a1->setCalendarDay(ca,30, true);
		m.addAgent(a1,ghr);


		Agent* a6 = new Agent("6",155, nbHoursWeek, Status::Beginner);
		a6->setService(ghr);
		a6->setCalendarDay(fp,23, true);
		m.addAgent(a6,ghr);

		Agent* a33 = new Agent("33",155, nbHoursWeek, Status::Confirmed);
		a33->setService(ghr);
		a33->setCalendarDay(cs, 24, true);
		a33->setCalendarDay(cs, 25, true);
		a33->setCalendarDay(cs, 26, true);
		m.addAgent(a33,ghr);

		Agent* a36 = new Agent("36",155, nbHoursWeek, Status::Confirmed);
		a36->setService(ghr);
		m.addAgent(a36,ghr);

		Agent* a40 = new Agent("40",155, nbHoursWeek, Status::Confirmed);
		a40->setService(ghr);
		a40->setCalendarDay(ca,0, true);
		m.addAgent(a40,ghr);

		Agent* a49 = new Agent("49",155, nbHoursWeek, Status::Confirmed);
		a49->setService(ghr);
		m.addAgent(a49,ghr);

		Agent* a57 = new Agent("57",155, nbHoursWeek, Status::Confirmed);
		a57->setService(ghr);
		a57->setCalendarDay(ca,0, true);
		a57->setCalendarDay(ca,1, true);
		m.addAgent(a57,ghr);

		Agent* a63 = new Agent("63",155, nbHoursWeek, Status::Confirmed);
		a63->setService(ghr);
		a63->setCalendarDay(ca,0, true);
		a63->setCalendarDay(ca,1, true);
		a63->setCalendarDay(ca,2, true);
		a63->setCalendarDay(ca,3, true);
		a63->setCalendarDay(ca,4, true);
		a63->setCalendarDay(ca,5, true);
		a63->setCalendarDay(ca,6, true);
		a63->setCalendarDay(ca,7, true);
		m.addAgent(a63,ghr);



		return m;

}


int main() {

	srand(time(0));

	Model m =  generateGhr();

	//auto m2 = heuristicSolver::iterative(m,100,300,3);
	auto m2 = heuristicSolver::iterative2(m, 20000, 3);

	cout << "bestScore " << heuristicSolver::check(&m2, false, true) << endl;

	m.printPlanning();
	cout << "---------------------------------" << endl;
	m2.printPlanning();

	/*
	m.printPlanning();
	cout << "---------------------------------" << endl;
	m2.printPlanning();

	//Model m3 = Model::generateModelInstance(Day::Sunday, 31, 25, 5, 10, 10, 48.0);
	
	for(auto s : m2.getServices()){
		for(auto a : m2.getAgentFrom(s)){
			cout << a->getId() << " Mois: " << a->getWorkingHoursMonth() << endl;
			for(int i=0;i<5;i++){
				cout << "Week " << i << ": " << a->getWorkingHoursWeek(m2.getFirstDay(),i) << endl;
			}
		}
	}

	//Test sur plusieurs models généré aléatoirements
	unsigned int nb = 1000;
	Model bestModel = m;
	int bScore = -500;
	int wScore = 10;
	for (int i = 0;i < nb;i++) {
		m2 = heuristicSolver::greedy(m);
		int tmp = heuristicSolver::check(&m2, false, false);
		if (tmp > bScore) {
			bestModel = m2;
			bScore = tmp;
		}
		if (tmp < wScore)
			wScore = tmp;
	}

	cout << heuristicSolver::check(&m2, false,true) << endl;
	cout << "worst " << wScore;*/
	
	string t;
	cin >> t;

	return 0;
}