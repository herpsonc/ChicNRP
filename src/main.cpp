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
#include "LP/LPSolver.h"

using namespace std;

void addConsecutiveSamePost(Agent* a, Post* p, int d_start, int d_end) {
	//permet d'ajouter le même post pendant d_end-d_start jours d'affilée à un agent
	for ( ; d_start <= d_end; d_start++) {
		a->setCalendarDay(p, d_start, true);
	}
}

Model generateGhr() {
		//Creation du Model pour le service GHR

		Model m = Model(6, 31, 25);

		Service* ghr = new Service("GHR");

		m.addAttribut("work");//0
		m.addAttribut("workL");//1
		m.addAttribut("day");//2
		m.addAttribut("dayL");//3
		m.addAttribut("night");//4
		m.addAttribut("rest");//5
		m.addAttribut("ca");//6

		m.addService(ghr);

		//Posts
		Post* jg = new Post("Jg", 12.25);
		jg->addAttribut(1);
		jg->addAttribut(0);
		jg->addAttribut(2);
		m.addPost(jg);
		jg->addAttribut(3);

		Post* ng = new Post("Ng", 12.25);
		ng->addAttribut(1);
		ng->addAttribut(0);
		ng->addAttribut(4);

		Post* mat = new Post("Mat", 12.25);
		mat->addAttribut(1);
		mat->addAttribut(2);
		mat->addAttribut(0);

		Post* repos = new Post("Repos", 0.0);
		repos->addAttribut(5);
		m.addPost(repos);

		Post* ca = new Post("Ca", 0.0);
		ca->addAttribut(5);
		ca->addAttribut(6);
		//m.addPost(ca);

		Post* oe = new Post("O/E", 7.5);
		oe->addAttribut(0);
		oe->addAttribut(2);
		//m.addPost(oe);

		Post* fp = new Post("FP", 7.5);
		fp->addAttribut(0);
		fp->addAttribut(2);
		//m.addPost(fp);

		Post* cs = new Post("CS", 7.5);
		cs->addAttribut(0);
		cs->addAttribut(2);
		//m.addPost(cs);

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
		m.addBasicConstraintsTo(ghr);

		//Si samedi Jg alors Jg dimanche + lundi
		auto v = vector<int>();
		v.push_back(3);
		auto v2 = vector<int>();
		v2.push_back(3);
		v2.push_back(3);
		ConstraintInvolved* cwjjj = new ConstraintInvolved(v, v2, 5, 1000);
		ghr->addConstraint(cwjjj);
		
		float nbHoursWeek = 60, nbHoursMonth = 155;

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
		// a6->setService(ghr);
		a6->setCalendarDay(fp,23, true);
		auto ip = vector<Post*>();
		ip.push_back(ng);
		a6->setImpossiblePosts(ip);
		m.addAgent(a6, ghr);

		Agent* a33 = new Agent("33", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		// a33->setService(ghr);
		a33->setCalendarDay(cs, 24, true);
		a33->setCalendarDay(cs, 25, true);
		a33->setCalendarDay(cs, 26, true);
		m.addAgent(a33,ghr);

		Agent* a36 = new Agent("36", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		// a36->setService(ghr);
		m.addAgent(a36,ghr);

		Agent* a40 = new Agent("40", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		// a40->setService(ghr);
		a40->setCalendarDay(ca,0, true);
		m.addAgent(a40,ghr);

		Agent* a49 = new Agent("49", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		// a49->setService(ghr);
		m.addAgent(a49,ghr);

		Agent* a57 = new Agent("57", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		// a57->setService(ghr);
		a57->setCalendarDay(ca,0, true);
		a57->setCalendarDay(ca,1, true);
		m.addAgent(a57,ghr);

		Agent* a63 = new Agent("63", nbHoursMonth, nbHoursWeek, Status::Confirmed);
		// a63->setService(ghr);
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
	js->addAttribut(1);
	js->addAttribut(0);
	js->addAttribut(2);
	Post* ns = new Post("Ns", 12.25);
	ns->addAttribut(1);
	ns->addAttribut(0);
	ns->addAttribut(4);

	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut(5);
	ca->addAttribut(6);

	Post* fp = new Post("FP", 7.5);
	fp->addAttribut(0);
	fp->addAttribut(2);

	Post* cs = new Post("CS", 7.5);
	cs->addAttribut(0);
	cs->addAttribut(2);

	m.setDefaultPost(repos);

	sdc->addPost(js);
	sdc->addPost(ns);
	sdc->addPost(repos);
	//ghr->addPost(ca);

	sdc->addPostRequired(js, 3);
	sdc->addPostRequired(ns, 1);
	


	//Contraintes
	m.addBasicConstraintsTo(sdc);


	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	auto ip = vector<Post*>();
	ip.push_back(ns);

	//Agents
	Agent* a9 = new Agent("9", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a9, sdc);


	Agent* a12 = new Agent("12", nbHoursMonth, nbHoursWeek, Status::Beginner);
	// a12->setService(sdc);
	a12->setCalendarDay(fp, 23, true);
	a12->setCalendarDay(ca, 29, true);
	a12->setCalendarDay(ca, 30, true);
	a12->setImpossiblePosts(ip);
	m.addAgent(a12, sdc);

	Agent* a17 = new Agent("17", 78, nbHoursWeek, Status::Beginner);
	// a17->setService(sdc);
	a17->setCalendarDay(ca, 3, true);
	a17->setCalendarDay(ca, 5, true);
	a17->setCalendarDay(ca, 10, true);
	a17->setCalendarDay(ca, 12, true);
	a17->setCalendarDay(ca, 17, true);
	a17->setCalendarDay(ca, 19, true);
	a17->setCalendarDay(ca, 24, true);
	a17->setCalendarDay(ca, 26, true);
	a17->setImpossiblePosts(ip);
	m.addAgent(a17, sdc);

	Agent* a34 = new Agent("34", nbHoursMonth, nbHoursWeek, Status::Beginner);
	// a34->setService(sdc);
	a34->setCalendarDay(fp, 1, true);
	a34->setCalendarDay(fp, 2, true);
	addConsecutiveSamePost(a34, ca, 3, 9);
	a34->setImpossiblePosts(ip);
	m.addAgent(a34, sdc);

	Agent* a39 = new Agent("39", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a39->setService(sdc);
	a39->setCalendarDay(ca, 27, true);
	a39->setCalendarDay(ca, 28, true);
	a39->setCalendarDay(ca, 29, true);
	a39->setCalendarDay(ca, 30, true);
	m.addAgent(a39, sdc);

	Agent* a46 = new Agent("46", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a46->setService(sdc);
	a46->setCalendarDay(fp, 23, true);
	addConsecutiveSamePost(a46, ca, 25, 30);
	m.addAgent(a46, sdc);

	Agent* a50 = new Agent("50", nbHoursMonth, nbHoursWeek, Status::Beginner);
	// a50->setService(sdc);
	a50->setCalendarDay(fp, 23, true);
	a50->setImpossiblePosts(ip);
	m.addAgent(a50, sdc);

	Agent* a61 = new Agent("61", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a61->setService(sdc);
	a61->setCalendarDay(ca, 0, true);
	addConsecutiveSamePost(a61, ca, 8, 14);
	m.addAgent(a61, sdc);


	Agent* a29 = new Agent("29", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a29->setService(sdc);
	m.addAgent(a29, sdc);

	Agent* a15 = new Agent("15", 78, nbHoursWeek, Status::Confirmed);
	// a15->setService(sdc);
	addConsecutiveSamePost(a15, ca, 0, 30);
	m.addAgent(a15, sdc);

	return m;

}

Model addServiceUK(Model m) {

	Service* UKang = new Service("UK");

	m.addService(UKang);

	Post* uk = new Post("UK", 12.25);
	uk->addAttribut(1);
	uk->addAttribut(0);
	uk->addAttribut(2);

	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut(5);
	ca->addAttribut(6);

	m.addPost(uk);
	m.addPost(repos);
	m.addPost(ca);
	UKang->addPostRequired(uk, 1);

	//Contraintes
	m.addBasicConstraintsTo(UKang);

	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	Agent* a59 = new Agent("59", nbHoursMonth, nbHoursWeek, Status::Beginner);
	a59->setCalendarDay(ca, 0, true);
	a59->setCalendarDay(ca, 1, true);
	addConsecutiveSamePost(a59, ca, 27, 30);
	m.addAgent(a59, UKang);

	Agent* a48 = new Agent("48", nbHoursMonth, nbHoursWeek, Status::Beginner);
	m.addAgent(a48, UKang);

	return m;
}

Model addServiceSDN(Model m) {

	//Ajout du service SDN à un modèle existant

	Service* sdn = new Service("SDN");

	m.addService(sdn);

	//Posts
	Post* jb = new Post("Jb", 12.25);
	jb->addAttribut(1);
	jb->addAttribut(0);
	jb->addAttribut(2);
	Post* nb = new Post("Nb", 12.25);
	nb->addAttribut(1);
	nb->addAttribut(0);
	nb->addAttribut(4);

	Post* u = new Post("U", 7.5);
	u->addAttribut(0);
	u->addAttribut(2);


	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut(5);
	ca->addAttribut(6);

	Post* fp = new Post("FP", 7.5);
	fp->addAttribut(0);
	fp->addAttribut(2);

	Post* eo = new Post("E/O", 7.5);
	eo->addAttribut(0);
	eo->addAttribut(2);

	//consultations
	Post* cs = new Post("CS", 7.5);
	cs->addAttribut(0);
	cs->addAttribut(2);

	Post* cgh = new Post("CGH", 7.5);
	cgh->addAttribut(0);
	cgh->addAttribut(2);

	Post* ort = new Post("ORT", 7.5);
	ort->addAttribut(0);
	ort->addAttribut(2);

	m.setDefaultPost(repos);

	sdn->addPost(jb);
	sdn->addPost(nb);
	sdn->addPost(u);
	sdn->addPost(repos);
	//ghr->addPost(ca);

	sdn->addPostRequired(jb, 4);
	sdn->addPostRequired(nb, 4);
	sdn->addPostRequired(u, 1);


	//Contraintes
	m.addBasicConstraintsTo(sdn);

	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	auto ip = vector<Post*>();
	//ip.push_back(nb);
	//????????????????

	//Agents
	Agent* a2 = new Agent("2", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a2->setService(sdn);
	a2->setImpossiblePosts(ip);
	m.addAgent(a2, sdn);

	Agent* a3 = new Agent("3", 125, nbHoursWeek, Status::Confirmed);
	// a3->setService(sdn);
	a3->setCalendarDay(ca, 3, true);
	a3->setCalendarDay(ca, 10, true);
	a3->setCalendarDay(ca, 17, true);
	a3->setCalendarDay(ca, 24, true);
	a3->setImpossiblePosts(ip);
	m.addAgent(a3, sdn);

	Agent* a4 = new Agent("4", 78, nbHoursWeek, Status::Confirmed);
	// a4->setService(sdn);
	addConsecutiveSamePost(a4, ca, 0, 7);
	a4->setCalendarDay(ca, 12, true);
	a4->setCalendarDay(ca, 19, true);
	a4->setCalendarDay(ca, 26, true);
	a4->setImpossiblePosts(ip);
	m.addAgent(a4, sdn);


	Agent* a7 = new Agent("7", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a7->setService(sdn);
	addConsecutiveSamePost(a7, ca, 18, 22);
	a7->setImpossiblePosts(ip);
	m.addAgent(a7, sdn);

	Agent* a10 = new Agent("10", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a10->setService(sdn);
	a10->setCalendarDay(cs, 1, true);
	a10->setCalendarDay(cgh, 4, true);
	a10->setCalendarDay(cs, 8, true);
	a10->setCalendarDay(cgh, 11, true);
	a10->setCalendarDay(cs, 15, true);
	a10->setCalendarDay(cgh, 18, true);
	addConsecutiveSamePost(a10, ca, 20, 21);
	a10->setCalendarDay(cs, 22, true);
	addConsecutiveSamePost(a10, ca, 25, 28);
	a10->setCalendarDay(cs, 29, true);
	a10->setImpossiblePosts(ip);
	m.addAgent(a10, sdn);


	Agent* a14 = new Agent("14", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a14->setService(sdn);
	addConsecutiveSamePost(a14, ca, 22, 24);
	a14->setImpossiblePosts(ip);
	m.addAgent(a14, sdn);

	Agent* a19 = new Agent("19", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a19->setService(sdn);
	addConsecutiveSamePost(a19, ca, 8, 14);
	addConsecutiveSamePost(a19, ca, 19, 21);
	a19->setImpossiblePosts(ip);
	m.addAgent(a19, sdn);

	Agent* a20 = new Agent("20", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a20->setService(sdn);
	a20->setCalendarDay(cs, 1, true);
	a20->setCalendarDay(ort, 2, true);
	a20->setCalendarDay(cs, 3, true);
	a20->setCalendarDay(ort, 5, true);
	a20->setCalendarDay(ort, 9, true);
	a20->setCalendarDay(ort, 12, true);
	a20->setCalendarDay(ort, 16, true);
	a20->setCalendarDay(ort, 19, true);
	a20->setCalendarDay(ort, 23, true);
	a20->setCalendarDay(ort, 26, true);
	a20->setCalendarDay(cs, 29, true);
	a20->setCalendarDay(ort, 30, true);
	a20->setImpossiblePosts(ip);
	m.addAgent(a20, sdn);

	Agent* a23 = new Agent("23", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a23->setService(sdn);
	a23->setCalendarDay(eo, 4, true);
	addConsecutiveSamePost(a23, ca, 5, 28);
	a23->setImpossiblePosts(ip);
	m.addAgent(a23, sdn);

	Agent* a24 = new Agent("24", 125, nbHoursWeek, Status::Confirmed);
	// a24->setService(sdn);
	a24->setCalendarDay(fp, 1, true);
	a24->setCalendarDay(fp, 2, true);
	a24->setCalendarDay(cs, 8, true);
	a24->setCalendarDay(cs, 10, true);
	a24->setCalendarDay(fp, 15, true);
	a24->setCalendarDay(fp, 23, true);
	a24->setImpossiblePosts(ip);
	m.addAgent(a24, sdn);


	Agent* a26 = new Agent("26", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a26->setService(sdn);
	addConsecutiveSamePost(a26, ca, 12, 14);
	a26->setImpossiblePosts(ip);
	m.addAgent(a26, sdn);

	Agent* a28 = new Agent("28", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a28->setService(sdn);
	a28->setImpossiblePosts(ip);
	m.addAgent(a28, sdn);

	Agent* a30 = new Agent("30", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a30->setService(sdn);
	a30->setImpossiblePosts(ip);
	m.addAgent(a30, sdn);

	Agent* a32 = new Agent("32", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a32->setService(sdn);
	a32->setCalendarDay(fp, 23, true);
	a32->setImpossiblePosts(ip);
	m.addAgent(a32, sdn);

	Agent* a38 = new Agent("38", 78, nbHoursWeek, Status::Confirmed);
	// a38->setService(sdn);
	a38->setCalendarDay(cgh, 2, true);
	addConsecutiveSamePost(a38, ca, 6, 14);
	a38->setCalendarDay(cgh, 16, true);
	a38->setCalendarDay(ca, 17, true);
	a38->setCalendarDay(cgh, 23, true);
	a38->setCalendarDay(cgh, 30, true);
	a38->setImpossiblePosts(ip);
	m.addAgent(a38, sdn);

	Agent* a51 = new Agent("51", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a51->setService(sdn);
	addConsecutiveSamePost(a51, ca, 2, 10);
	a51->setImpossiblePosts(ip);
	m.addAgent(a51, sdn);

	Agent* a52 = new Agent("52", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a52->setService(sdn);
	a52->setCalendarDay(ca, 0, true);
	addConsecutiveSamePost(a52, ca, 6, 14);
	a52->setImpossiblePosts(ip);
	m.addAgent(a52, sdn);

	Agent* a53 = new Agent("53", 125, nbHoursWeek, Status::Confirmed);
	// a53->setService(sdn);
	addConsecutiveSamePost(a53, ca, 0, 7);
	a53->setCalendarDay(ca, 30, true);
	a53->setImpossiblePosts(ip);
	m.addAgent(a53, sdn);

	Agent* a55 = new Agent("55", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a55->setService(sdn);
	a55->setCalendarDay(ca, 3, true);
	a55->setCalendarDay(cgh, 5, true);
	a55->setCalendarDay(ca, 10, true);
	a55->setCalendarDay(cgh, 12, true);
	a55->setCalendarDay(ca, 17, true);
	a55->setCalendarDay(cgh, 19, true);
	a55->setCalendarDay(ca, 24, true);
	a55->setCalendarDay(cgh, 26, true);
	addConsecutiveSamePost(a55, ca, 27, 29);
	a55->setImpossiblePosts(ip);
	m.addAgent(a55, sdn);

	Agent* a58 = new Agent("58", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a58->setService(sdn);
	addConsecutiveSamePost(a58, cs, 10, 12);
	addConsecutiveSamePost(a58, ca, 13, 21);
	a58->setImpossiblePosts(ip);
	m.addAgent(a58, sdn);

	Agent* a62 = new Agent("62", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a62->setService(sdn);
	addConsecutiveSamePost(a62, ca, 0, 2);
	addConsecutiveSamePost(a62, ca, 12, 13);
	a62->setImpossiblePosts(ip);
	m.addAgent(a62, sdn);

	Agent* a64 = new Agent("64", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a64->setService(sdn);
	addConsecutiveSamePost(a64, ca, 0, 1);
	a64->setImpossiblePosts(ip);
	m.addAgent(a64, sdn);

	Agent* a8 = new Agent("8", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a8->setService(sdn);
	addConsecutiveSamePost(a8, ca, 0, 30);
	a8->setImpossiblePosts(ip);
	m.addAgent(a8, sdn);

	Agent* a21 = new Agent("21", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a21->setService(sdn);
	addConsecutiveSamePost(a21, ca, 0, 30);
	a21->setImpossiblePosts(ip);
	m.addAgent(a21, sdn);

	Agent* a27 = new Agent("27", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a27->setService(sdn);
	addConsecutiveSamePost(a27, ca, 0, 30);
	a27->setImpossiblePosts(ip);
	m.addAgent(a27, sdn);

	Agent* a41 = new Agent("41", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a41->setService(sdn);
	addConsecutiveSamePost(a41, ca, 0, 30);
	a41->setImpossiblePosts(ip);
	m.addAgent(a41, sdn);

	Agent* a42 = new Agent("42", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a42->setService(sdn);
	addConsecutiveSamePost(a42, ca, 0, 30);
	a42->setImpossiblePosts(ip);
	m.addAgent(a42, sdn);

	Agent* a45 = new Agent("45", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a45->setService(sdn);
	a45->setCalendarDay(ca, 0, true);
	a45->setImpossiblePosts(ip);
	m.addAgent(a45, sdn);

	Agent* a60 = new Agent("60", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a60->setService(sdn);
	a60->setImpossiblePosts(ip);
	m.addAgent(a60, sdn);

	Agent* a54 = new Agent("54", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a54->setService(sdn);
	a54->setCalendarDay(fp, 23, true);
	a54->setImpossiblePosts(ip);
	m.addAgent(a54, sdn);

	Agent* a37 = new Agent("37", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a37->setService(sdn);
	a37->setCalendarDay(fp, 23, true);
	a37->setImpossiblePosts(ip);
	m.addAgent(a37, sdn);

	return m;
}

Model addServiceCS(Model m) {

	//Ajout du service CS à un modèle existant
	Service* cs_service = new Service("CS");

	m.addService(cs_service);

	//Posts
	Post* cs = new Post("CS", 7.5);
	cs->addAttribut(0);
	cs->addAttribut(2);

	Post* cgh = new Post("CGH", 7.5);
	cgh->addAttribut(0);
	cgh->addAttribut(2);

	Post* ban = new Post("Ban", 7.5);
	ban->addAttribut(0);
	ban->addAttribut(2);

	Post* acu = new Post("Acu", 7.5);
	acu->addAttribut(0);
	acu->addAttribut(2);

	Post* orientation = new Post("O", 7.5);
	orientation->addAttribut(0);
	orientation->addAttribut(2);

	Post* diag = new Post("D", 7.5);
	diag->addAttribut(0);
	diag->addAttribut(2);

	Post* ort = new Post("Ort", 7.5);
	ort->addAttribut(0);
	ort->addAttribut(2);



	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut(5);
	ca->addAttribut(6);

	Post* fp = new Post("FP", 7.5);
	fp->addAttribut(0);
	fp->addAttribut(2);

	Post* eo = new Post("E/O", 7.5);
	eo->addAttribut(0);
	eo->addAttribut(2);

	m.setDefaultPost(repos);

	cs_service->addPost(cs);
	cs_service->addPost(cgh);
	cs_service->addPost(ban);
	cs_service->addPost(acu);
	cs_service->addPost(orientation);
	cs_service->addPost(diag);
	cs_service->addPost(ort);
	cs_service->addPost(eo);
	cs_service->addPost(repos);

	cs_service->addPostRequired(orientation, 2, 0);
	cs_service->addPostRequired(orientation, 2, 1);
	cs_service->addPostRequired(orientation, 2, 2);
	cs_service->addPostRequired(orientation, 2, 3);
	cs_service->addPostRequired(orientation, 2, 4);
	//cs_service->addPostRequired(diag, 1);
	cs_service->addPostRequired(ban, 1, 0);
	cs_service->addPostRequired(ban, 1, 1);

	cs_service->addPostRequired(ban, 1, 3);
	cs_service->addPostRequired(ban, 1, 4);


	//Contraintes
	m.addBasicConstraintsTo(cs_service);


	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	auto ip = vector<Post*>();
	//ip.push_back(cs);

	//Agents

	Agent* a31 = new Agent("31", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a31->setService(cs_service);
	addConsecutiveSamePost(a31, diag, 1, 5);
	addConsecutiveSamePost(a31, diag, 8, 12);
	addConsecutiveSamePost(a31, diag, 15, 19);
	addConsecutiveSamePost(a31, diag, 22, 26);
	addConsecutiveSamePost(a31, diag, 29, 30);
	a31->setImpossiblePosts(ip);
	m.addAgent(a31, cs_service);

	Agent* a11 = new Agent("11", 78, nbHoursWeek, Status::Confirmed);
	// a11->setService(cs_service);
	a11->setCalendarDay(ca, 1, true);
	a11->setCalendarDay(cs, 2, true);
	a11->setCalendarDay(ca, 3, true);
	a11->setCalendarDay(ca, 8, true);
	a11->setCalendarDay(cs, 9, true);
	a11->setCalendarDay(ca, 10, true);
	a11->setCalendarDay(ca, 15, true);
	a11->setCalendarDay(cs, 16, true);
	a11->setCalendarDay(ca, 17, true);
	a11->setCalendarDay(ca, 22, true);
	a11->setCalendarDay(cs, 23, true);
	a11->setCalendarDay(ca, 24, true);
	a11->setCalendarDay(ca, 29, true);
	a11->setCalendarDay(cs, 30, true);
	a11->setImpossiblePosts(ip);
	m.addAgent(a11, cs_service);

	Agent* a13 = new Agent("13", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a13->setService(cs_service);
	a13->setCalendarDay(cs, 1, true);
	a13->setCalendarDay(ban, 2, true);
	a13->setCalendarDay(eo, 3, true);
	a13->setCalendarDay(cs, 8, true);
	a13->setCalendarDay(ban, 9, true);
	a13->setCalendarDay(eo, 10, true);
	a13->setCalendarDay(cs, 22, true);
	a13->setCalendarDay(fp, 23, true);
	a13->setCalendarDay(eo, 24, true);
	a13->setCalendarDay(cs, 29, true);
	a13->setCalendarDay(ban, 30, true);
	addConsecutiveSamePost(a13, ca, 11, 21);
	a13->setImpossiblePosts(ip);
	m.addAgent(a13, cs_service);

	Agent* a22 = new Agent("22", 78, nbHoursWeek, Status::Confirmed);
	// a22->setService(cs_service);
	a22->setCalendarDay(cgh, 1, true);
	a22->setCalendarDay(acu, 4, true);
	a22->setCalendarDay(cgh, 8, true);
	a22->setCalendarDay(acu, 11, true);
	addConsecutiveSamePost(a22, ca, 13, 14);
	a22->setCalendarDay(cgh, 15, true);
	a22->setCalendarDay(cgh, 22, true);
	a22->setCalendarDay(acu, 25, true);
	a22->setCalendarDay(cgh, 29, true);
	a22->setImpossiblePosts(ip);
	m.addAgent(a22, cs_service);

	Agent* a35 = new Agent("35", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a35->setService(cs_service);
	addConsecutiveSamePost(a35, ca, 15, 19);
	a35->setImpossiblePosts(ip);
	m.addAgent(a35, cs_service);


	Agent* a44 = new Agent("44", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a44->setService(cs_service);
	ip = vector<Post*>();
	ip.push_back(orientation);
	a44->setImpossiblePosts(ip);
	addConsecutiveSamePost(a44, ca, 0, 3);
	addConsecutiveSamePost(a44, ca, 20, 21);
	a44->setImpossiblePosts(ip);
	m.addAgent(a44, cs_service);

	return m;

}

Model addServiceExplo(Model m) {
	Service* explo = new Service("Explo");

	m.addService(explo);

	Post* explo_fonctionnelle = new Post("+", 7.5);
	explo_fonctionnelle->addAttribut(0);
	explo_fonctionnelle->addAttribut(2);

	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);

	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut(5);
	ca->addAttribut(6);

	explo->addPost(explo_fonctionnelle);
	explo->addPost(repos);

	explo->addPostRequired(explo_fonctionnelle, 2, 0);
	explo->addPostRequired(explo_fonctionnelle, 2, 1);
	explo->addPostRequired(explo_fonctionnelle, 2, 2);
	explo->addPostRequired(explo_fonctionnelle, 2, 3);
	explo->addPostRequired(explo_fonctionnelle, 2, 4);

	m.addBasicConstraintsTo(explo);


	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	Agent* a43 = new Agent("43", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a43->setService(cs_service);
	addConsecutiveSamePost(a43, ca, 27, 30);
	m.addAgent(a43, explo);

	Agent* a5 = new Agent("5", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a5->setService(cs_service);
	m.addAgent(a5, explo);

	return m;
}

Model addServicePool(Model m) {

	//Ajout du service Pool à un modèle existant

	Service* pool = new Service("Pool");

	m.addService(pool);

	//Posts
	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut(5);
	ca->addAttribut(6);

	Post* cs = new Post("CS", 7.5);
	cs->addAttribut(0);
	cs->addAttribut(2);

	Post* fp = new Post("FP", 7.5);
	fp->addAttribut(0);
	fp->addAttribut(2);

	m.setDefaultPost(repos);

	pool->addPost(cs);
	pool->addPost(fp);
	pool->addPost(repos);
	//pas de posts required dans le pool

	//Contraintes
	m.addBasicConstraintsTo(pool);

	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	auto ip = vector<Post*>();
	//ip.push_back(cs);

	//Agents

	Agent* a16 = new Agent("16", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a16->setService(pool);
	addConsecutiveSamePost(a16, cs, 3, 5);
	addConsecutiveSamePost(a16, ca, 13, 14);
	addConsecutiveSamePost(a16, ca, 26, 28);
	a16->setImpossiblePosts(ip);
	m.addAgent(a16, pool);

	Agent* a25 = new Agent("25", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a25->setService(pool);
	addConsecutiveSamePost(a25, ca, 20, 30);
	a25->setImpossiblePosts(ip);
	m.addAgent(a25, pool);

	Agent* a47 = new Agent("47", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a47->setService(pool);
	addConsecutiveSamePost(a47, fp, 10, 12);
	a47->setCalendarDay(cs, 22, true);
	a47->setCalendarDay(fp, 23, true);
	a47->setCalendarDay(cs, 24, true);
	a47->setImpossiblePosts(ip);
	m.addAgent(a47, pool);

	Agent* a18 = new Agent("18", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a18->setService(pool);
	addConsecutiveSamePost(a18, ca, 10, 16);
	addConsecutiveSamePost(a18, cs, 17, 19);
	a18->setImpossiblePosts(ip);
	m.addAgent(a18, pool);

	return m;

}


int main() {

	//Important pour initialiser l'aléatoire
	srand(time(0));

	//modèle avec les (pré)données de Mars 2020 du CHIC
	Model m = generateGhr();
	
	//m.getServices()[0]->loadPredefinedPlanning("GHRTrame.csv", m.getNbDays(), m.getFirstDay());
	m.getServices()[0]->loadPredefinedPlanning("data/prePlannings/GHR.csv", m.getNbDays(), m.getFirstDay());
	m.getServices()[0]->getPredefinedPlanning()->printPlanning();
	cout << "----------------------------------" << endl;
	

	m = addServiceSDC(m);
	m.getServices()[1]->loadPredefinedPlanning("data/prePlannings/SDC.csv", m.getNbDays(), m.getFirstDay());
	m.getServices()[1]->getPredefinedPlanning()->printPlanning();
	cout << "----------------------------------" << endl;
	m = addServiceSDN(m);
	m.getServices()[2]->loadPredefinedPlanning("data/prePlannings/SDN.csv", m.getNbDays(), m.getFirstDay());
	m.getServices()[2]->getPredefinedPlanning()->printPlanning();
	cout << "----------------------------------" << endl;
	m = addServicePool(m);
	m = addServiceCS(m);
	m = addServiceExplo(m);
	m = addServiceUK(m);
	m.getServices()[6]->loadPredefinedPlanning("data/prePlannings/UK.csv", m.getNbDays(), m.getFirstDay());
	m.printPlanning();
	
	//auto m2 = heuristicSolver::predefinedGreedy(m);
	//m2.printPlanning();
	auto m2 = heuristicSolver::iterativeFast(m, 1000000, 5);
	auto m3 = heuristicSolver::iterativePool(m2, 1000000, 2,m2.getServices()[3]);

	m3.printPlanning();
	//m2.getValuation()->print();
	cout << m2.getConstraintInformations()<< endl;
	m3.generateXlsx("AllService2.xml");

	m3.generateEmptyValuation();
	HeuristicToolBox::checkAllFast(&m3);
	cout << "scooore " << m3.getValuation()->getScore();
	cout << m3.getConstraintInformations() << endl;

	/*Model m2 = Model::generateModelInstance(2, 30, 25, 6, 15, 70, 60.0, 155);
	m2.printPlanning();
	m2.generateXlsx("service_g6_PL.xlsx");
	m2 = LPSolver::linearProgram(m2);*/

	string t;
	cin >> t;

	return 0;
}