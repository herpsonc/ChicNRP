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

void addConsecutiveSamePost(Agent* a, Post* p, int d_start, int d_end) {
	//permet d'ajouter le même post pendant d_end-d_start jours d'affilé à un agent
	for (d_start; d_start <= d_end; d_start++) {
		a->setCalendarDay(p, d_start, true);
	}
}

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


		float nbHoursWeek = 48.0, nbHoursMonth = 155;

		//Agents
		Agent* a1 = new Agent("1", nbHoursMonth, nbHoursWeek,  Status::Confirmed);
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


		Agent* a6 = new Agent("6", nbHoursMonth, nbHoursWeek, Status::Beginner);
		a6->setService(ghr);
		a6->setCalendarDay(fp,23, true);
		m.addAgent(a6,ghr);

		Agent* a33 = new Agent("33", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		a33->setService(ghr);
		a33->setCalendarDay(cs, 24, true);
		a33->setCalendarDay(cs, 25, true);
		a33->setCalendarDay(cs, 26, true);
		m.addAgent(a33,ghr);

		Agent* a36 = new Agent("36", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		a36->setService(ghr);
		m.addAgent(a36,ghr);

		Agent* a40 = new Agent("40", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		a40->setService(ghr);
		a40->setCalendarDay(ca,0, true);
		m.addAgent(a40,ghr);

		Agent* a49 = new Agent("49", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		a49->setService(ghr);
		m.addAgent(a49,ghr);

		Agent* a57 = new Agent("57", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		a57->setService(ghr);
		a57->setCalendarDay(ca,0, true);
		a57->setCalendarDay(ca,1, true);
		m.addAgent(a57,ghr);

		Agent* a63 = new Agent("63", nbHoursMonth, nbHoursWeek, Status::Confirmed);
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

Model addServiceSDC(Model m) {

	//Ajout du service SDC à un modèle existant

	Service* sdc = new Service("SDC");

	m.addService(sdc);

	//Posts
	Post* js = new Post("Js", 12.25);
	js->addAttribut("workL");
	js->addAttribut("work");
	js->addAttribut("day");
	Post* ns = new Post("Js", 12.25);
	ns->addAttribut("workL");
	ns->addAttribut("work");
	ns->addAttribut("night");

	Post* uk = new Post("UK", 12.25);
	uk->addAttribut("workL");
	uk->addAttribut("work");
	uk->addAttribut("day");


	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut("rest");
	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut("rest");
	ca->addAttribut("ca");

	Post* fp = new Post("FP", 7.5);
	fp->addAttribut("work");
	fp->addAttribut("day");

	Post* cs = new Post("CS", 7.5);
	cs->addAttribut("work");
	cs->addAttribut("day");

	m.setDefaultPost(repos);

	sdc->addPost(js);
	sdc->addPost(ns);
	sdc->addPost(uk);
	sdc->addPost(repos);
	//ghr->addPost(ca);

	sdc->addPostRequired(js, 3);
	sdc->addPostRequired(ns, 1);
	sdc->addPostRequired(uk, 1);


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
	ConstraintSeqMinMax* cwe = new ConstraintSeqMinMax(Day::Saturday, MinMax::Min, 1, v, 1);

	sdc->addConstraint(cJN);
	sdc->addConstraint(c3N);
	sdc->addConstraint(crn);
	sdc->addConstraint(cnr);
	sdc->addConstraint(cwe);


	float nbHoursWeek = 48.0, nbHoursMonth = 155;

	//Agents
	Agent* a9 = new Agent("9", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a9, sdc);


	Agent* a12 = new Agent("12", nbHoursMonth, nbHoursWeek, Status::Beginner);
	a12->setService(sdc);
	a12->setCalendarDay(fp, 23, true);
	a12->setCalendarDay(ca, 29, true);
	a12->setCalendarDay(ca, 30, true);
	m.addAgent(a12, sdc);

	Agent* a17 = new Agent("17", nbHoursMonth, nbHoursWeek, Status::Beginner);
	a17->setService(sdc);
	a17->setCalendarDay(ca, 3, true);
	a17->setCalendarDay(ca, 5, true);
	a17->setCalendarDay(ca, 10, true);
	a17->setCalendarDay(ca, 12, true);
	a17->setCalendarDay(ca, 17, true);
	a17->setCalendarDay(ca, 24, true);
	a17->setCalendarDay(ca, 26, true);
	m.addAgent(a17, sdc);

	Agent* a34 = new Agent("34", nbHoursMonth, nbHoursWeek, Status::Beginner);
	a34->setService(sdc);
	a34->setCalendarDay(fp, 1, true);
	a34->setCalendarDay(fp, 2, true);
	addConsecutiveSamePost(a34, ca, 3, 9);
	m.addAgent(a34, sdc);

	Agent* a39 = new Agent("39", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a39->setService(sdc);
	a39->setCalendarDay(ca, 27, true);
	a39->setCalendarDay(ca, 28, true);
	a39->setCalendarDay(ca, 29, true);
	a39->setCalendarDay(ca, 30, true);
	m.addAgent(a39, sdc);

	Agent* a46 = new Agent("46", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a46->setService(sdc);
	a46->setCalendarDay(fp, 23, true);
	addConsecutiveSamePost(a46, ca, 25, 30);
	m.addAgent(a46, sdc);

	Agent* a50 = new Agent("50", nbHoursMonth, nbHoursWeek, Status::Beginner);
	a50->setService(sdc);
	a50->setCalendarDay(fp, 23, true);
	m.addAgent(a50, sdc);

	Agent* a61 = new Agent("61", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a61->setService(sdc);
	a61->setCalendarDay(ca, 0, true);
	addConsecutiveSamePost(a61, ca, 8, 14);
	m.addAgent(a61, sdc);


	Agent* a29 = new Agent("29", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a29->setService(sdc);
	m.addAgent(a29, sdc);

	Agent* a59 = new Agent("59", nbHoursMonth, nbHoursWeek, Status::Beginner);
	a59->setService(sdc);
	a59->setCalendarDay(ca, 0, true);
	a59->setCalendarDay(ca, 1, true);
	addConsecutiveSamePost(a59, ca, 27, 30);
	m.addAgent(a59, sdc);

	Agent* a48 = new Agent("48", nbHoursMonth, nbHoursWeek, Status::Beginner);
	a48->setService(sdc); 
	m.addAgent(a48, sdc);

	Agent* a15 = new Agent("15", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a15->setService(sdc);
	addConsecutiveSamePost(a15, ca, 0, 30);
	m.addAgent(a15, sdc);

	return m;

}


int main() {

	srand(time(0));

	Model m =  generateGhr();
	m = addServiceSDC(m); //ajoute le service SDC au modèle
	
	//auto m2 = heuristicSolver::iterative(m,100,300,3);
	
	//generateModelInstance(Day firstDay, int nbDays, float overtime, int nbServices, int nbPosts, int nbAgents, float nbHoursWeek, float nbHoursMonth, int nbAgentsPerService, int nbPostsPerService)

	auto m_1servicePetit = Model::generateModelInstance(Day::Monday, 31, 25, 1, 4, 7, 48.0, 155); //1 petit service, pas trop contraints
	auto m_1serviceTresGrand = Model::generateModelInstance(Day::Thursday, 30, 25, 1, 10, 30, 48.0, 155); //1 grand service, pas trop contraints

	auto m_2servicesPetits = Model::generateModelInstance(Day::Sunday, 31, 25, 2, 5, 10, 48.0, 155); //2 petits services, pas trop contraints
	auto m_2servicesFevrierBissextile = Model::generateModelInstance(Day::Saturday, 29, 0, 2, 5, 10, 48.0, 140); //2 petits services, fevrier, 0 heures supps possibles


	auto m_3servicesEte = Model::generateModelInstance(Day::Sunday, 31, 10, 3, 6, 15, 48.0, 155, -1, -1, 5, 100); //3 petits services avec beacoup de congés (simulation vacances d'été potentielle), 10h supps max

	auto m_6servicesPetits = Model::generateModelInstance(Day::Sunday, 31, 25, 6, 7, 20, 48.0, 155); //6 petits services (7 postes, 20 agents)
	auto m_6servicesGrands = Model::generateModelInstance(Day::Wednesday, 30, 25, 6, 20, 70, 48.0, 155); //6 grands services (20 postes, 70 agents)

	//auto m3 = heuristicSolver::greedy(m2);
	//auto m4 = heuristicSolver::iterative2(m2, 20000, 3);

	//cout << "bestScore " << heuristicSolver::check(&m4, false, true) << endl;

	m_3servicesEte.printPlanning();
	cout << "---------------------------------" << endl;
	//m3.printPlanning();
	//cout << "---------------------------------" << endl;
	//m4.printPlanning();

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