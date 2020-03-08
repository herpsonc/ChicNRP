//============================================================================
// Name        : PANDROIDE.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <iostream>
#include <vector>
#include "model/Agent.h"
#include "model/constraint/Constraint.h"
#include "model/constraint/ConstraintDaysSeq.h"
#include "model/Model.h"
using namespace std;

Model generateGhr() {

		//Creation du model pour le service GHR
		Model m = Model(Day::Sunday, 31, 25);

		Service* ghr = new Service("GHR");

		//Posts
		Post* jg = new Post("Jg", 12.25);
		jg->addAttribut("work");
		jg->addAttribut("day");
		Post* ng = new Post("Ng", 12.25);
		ng->addAttribut("work");
		ng->addAttribut("night");
		Post* mat = new Post("Mat", 12.25);
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

		ghr->addPost(jg);
		ghr->addPost(ng);
		ghr->addPost(mat);
		ghr->addPost(repos);

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
		v.push_back("work");
		v.push_back("work");
		v.push_back("work");
		ConstraintDaysSeq* c3N = new ConstraintDaysSeq(v, 1);

		//Pas de nuit avant un congé posé
		v = vector<string>();
		v.push_back("night");
		v.push_back("ca");
		ConstraintDaysSeq* crn = new ConstraintDaysSeq(v, 1);

		ghr->addConstraint(cJN);
		ghr->addConstraint(c3N);
		ghr->addConstraint(crn);


		//Agents
		Agent* a1 = new Agent("1",155, Status::Confirmed);
		a1->setCalendarDay(oe,3);
		a1->setCalendarDay(oe,10);
		a1->setCalendarDay(oe,17);
		a1->setCalendarDay(oe,24);
		a1->setCalendarDay(ca,0);
		a1->setCalendarDay(ca,1);
		a1->setCalendarDay(ca,2);
		a1->setCalendarDay(ca,25);
		a1->setCalendarDay(ca,26);
		a1->setCalendarDay(ca,27);
		a1->setCalendarDay(ca,28);
		a1->setCalendarDay(ca,29);
		a1->setCalendarDay(ca,30);
		m.addAgent(a1);

		Agent* a6 = new Agent("6",155, Status::Beginner);
		a6->setService(ghr);
		a6->setCalendarDay(fp,23);
		m.addAgent(a6);

		Agent* a33 = new Agent("33",155, Status::Confirmed);
		a33->setService(ghr);
		a33->setCalendarDay(cs, 24);
		a33->setCalendarDay(cs, 25);
		a33->setCalendarDay(cs, 26);
		m.addAgent(a33);

		Agent* a36 = new Agent("36",155, Status::Confirmed);
		a36->setService(ghr);
		m.addAgent(a36);

		Agent* a40 = new Agent("40",155, Status::Confirmed);
		a40->setService(ghr);
		a40->setCalendarDay(ca,0);
		m.addAgent(a40);

		Agent* a49 = new Agent("49",155, Status::Confirmed);
		a49->setService(ghr);
		m.addAgent(a49);

		Agent* a57 = new Agent("57",155, Status::Confirmed);
		a57->setService(ghr);
		a57->setCalendarDay(ca,0);
		a57->setCalendarDay(ca,1);
		m.addAgent(a57);

		Agent* a63 = new Agent("63",155, Status::Confirmed);
		a63->setService(ghr);
		a63->setCalendarDay(ca,0);
		a63->setCalendarDay(ca,1);
		a63->setCalendarDay(ca,2);
		a63->setCalendarDay(ca,3);
		a63->setCalendarDay(ca,4);
		a63->setCalendarDay(ca,5);
		a63->setCalendarDay(ca,6);
		a63->setCalendarDay(ca,7);
		m.addAgent(a63);



		return m;

}


int main() {

	Model m =  generateGhr();

	m.printPlanning();


	return 0;
}
