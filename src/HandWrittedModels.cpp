#include "HandWrittedModels.h"

void HandWrittedModels::addConsecutiveSamePost(Agent* a, Post* p, int d_start, int d_end)
{
	//permet d'ajouter le même post pendant d_end-d_start jours d'affilée à un agent
	for (; d_start <= d_end; d_start++) {
		a->setCalendarDay(p, d_start, true);
	}
}

Model HandWrittedModels::generateGhrMarch20()
{
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

	Post* ca = new Post("Ca", 7.5, false);
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

	//Si dimanche Jg alors Jg lundi
	v = vector<int>();
	v.push_back(3);
	v2 = vector<int>();
	v2.push_back(3);
	ConstraintInvolved* cwjj = new ConstraintInvolved(v, v2, 6, 1000);
	ghr->addConstraint(cwjj);

	float nbHoursWeek = 60, nbHoursMonth = 155;

	//Agents
	Agent* a1 = new Agent("1", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a1->setCalendarDay(oe, 3, true);
	a1->setCalendarDay(oe, 10, true);
	a1->setCalendarDay(oe, 17, true);
	a1->setCalendarDay(oe, 24, true);
	a1->setCalendarDay(ca, 0, true);
	a1->setCalendarDay(ca, 1, true);
	a1->setCalendarDay(ca, 2, true);
	a1->setCalendarDay(ca, 25, true);
	a1->setCalendarDay(ca, 26, true);
	a1->setCalendarDay(ca, 27, true);
	a1->setCalendarDay(ca, 28, true);
	a1->setCalendarDay(ca, 29, true);
	a1->setCalendarDay(ca, 30, true);
	m.addAgent(a1, ghr);


	Agent* a6 = new Agent("6", nbHoursMonth, nbHoursWeek, Status::Beginner);
	// a6->setService(ghr);
	a6->setCalendarDay(fp, 23, true);
	auto ip = vector<Post*>();
	ip.push_back(ng);
	a6->setImpossiblePosts(ip);
	m.addAgent(a6, ghr);

	Agent* a33 = new Agent("33", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a33->setService(ghr);
	a33->setCalendarDay(cs, 24, true);
	a33->setCalendarDay(cs, 25, true);
	a33->setCalendarDay(cs, 26, true);
	m.addAgent(a33, ghr);

	Agent* a36 = new Agent("36", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a36->setService(ghr);
	m.addAgent(a36, ghr);

	Agent* a40 = new Agent("40", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a40->setService(ghr);
	a40->setCalendarDay(ca, 0, true);
	m.addAgent(a40, ghr);

	Agent* a49 = new Agent("49", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a49->setService(ghr);
	m.addAgent(a49, ghr);

	Agent* a57 = new Agent("57", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a57->setService(ghr);
	a57->setCalendarDay(ca, 0, true);
	a57->setCalendarDay(ca, 1, true);
	m.addAgent(a57, ghr);

	Agent* a63 = new Agent("63", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a63->setService(ghr);
	a63->setCalendarDay(ca, 0, true);
	a63->setCalendarDay(ca, 1, true);
	a63->setCalendarDay(ca, 2, true);
	a63->setCalendarDay(ca, 3, true);
	a63->setCalendarDay(ca, 4, true);
	a63->setCalendarDay(ca, 5, true);
	a63->setCalendarDay(ca, 6, true);
	a63->setCalendarDay(ca, 7, true);
	m.addAgent(a63, ghr);

	return m;
}

Model HandWrittedModels::addServiceSDCMarch20(Model m)
{

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
	Post* ca = new Post("Ca", 7.5, false);
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
	a17->setCalendarDay(repos, 3, true);
	a17->setCalendarDay(repos, 5, true);
	a17->setCalendarDay(repos, 10, true);
	a17->setCalendarDay(repos, 12, true);
	a17->setCalendarDay(repos, 17, true);
	a17->setCalendarDay(repos, 19, true);
	a17->setCalendarDay(repos, 24, true);
	a17->setCalendarDay(repos, 26, true);
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

Model HandWrittedModels::addServiceUKMarch20(Model m)
{
	Service* UKang = new Service("UK");

	m.addService(UKang);

	Post* uk = new Post("UK", 12.25);
	uk->addAttribut(1);
	uk->addAttribut(0);
	uk->addAttribut(2);

	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 7.5, false);
	ca->addAttribut(5);
	ca->addAttribut(6);

	UKang->addPost(uk);
	UKang->addPost(repos);
	UKang->addPost(ca);
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

Model HandWrittedModels::addServiceSDNMarch20(Model m)
{
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
	Post* ca = new Post("Ca", 7.5, false);
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

	//Agents
	Agent* a2 = new Agent("2", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a2, sdn);

	Agent* a3 = new Agent("3", 125, nbHoursWeek, Status::Confirmed);
	a3->setCalendarDay(repos, 3, true);
	a3->setCalendarDay(repos, 10, true);
	a3->setCalendarDay(repos, 17, true);
	a3->setCalendarDay(repos, 24, true);
	m.addAgent(a3, sdn);

	Agent* a4 = new Agent("4", 78, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a4, ca, 0, 4);
	a4->setCalendarDay(repos, 5, true);
	addConsecutiveSamePost(a4, ca, 6, 7);
	a4->setCalendarDay(repos, 12, true);
	a4->setCalendarDay(repos, 19, true);
	a4->setCalendarDay(repos, 26, true);
	m.addAgent(a4, sdn);


	Agent* a7 = new Agent("7", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a7, ca, 18, 22);
	m.addAgent(a7, sdn);

	Agent* a10 = new Agent("10", nbHoursMonth, nbHoursWeek, Status::Confirmed);
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
	m.addAgent(a10, sdn);


	Agent* a14 = new Agent("14", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a14, ca, 22, 24);
	m.addAgent(a14, sdn);

	Agent* a19 = new Agent("19", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a19, ca, 8, 14);
	addConsecutiveSamePost(a19, ca, 19, 21);
	m.addAgent(a19, sdn);

	Agent* a20 = new Agent("20", nbHoursMonth, nbHoursWeek, Status::Confirmed);
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
	m.addAgent(a20, sdn);

	Agent* a23 = new Agent("23", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a23->setCalendarDay(eo, 4, true);
	addConsecutiveSamePost(a23, ca, 5, 28);
	m.addAgent(a23, sdn);

	Agent* a24 = new Agent("24", 125, nbHoursWeek, Status::Confirmed);
	a24->setCalendarDay(fp, 1, true);
	a24->setCalendarDay(fp, 2, true);
	a24->setCalendarDay(cs, 8, true);
	a24->setCalendarDay(cs, 10, true);
	a24->setCalendarDay(fp, 15, true);
	a24->setCalendarDay(fp, 23, true);
	m.addAgent(a24, sdn);


	Agent* a26 = new Agent("26", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a26, ca, 12, 14);
	m.addAgent(a26, sdn);

	Agent* a28 = new Agent("28", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a28, sdn);

	Agent* a30 = new Agent("30", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a30, sdn);

	Agent* a32 = new Agent("32", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a32->setCalendarDay(fp, 23, true);
	m.addAgent(a32, sdn);

	Agent* a38 = new Agent("38", 78, nbHoursWeek, Status::Confirmed);
	a38->setCalendarDay(cgh, 2, true);
	addConsecutiveSamePost(a38, ca, 6, 14);
	a38->setCalendarDay(cgh, 16, true);
	a38->setCalendarDay(ca, 17, true);
	a38->setCalendarDay(cgh, 23, true);
	a38->setCalendarDay(cgh, 30, true);
	m.addAgent(a38, sdn);

	Agent* a51 = new Agent("51", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a51, ca, 2, 10);
	m.addAgent(a51, sdn);

	Agent* a52 = new Agent("52", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a52->setCalendarDay(ca, 0, true);
	addConsecutiveSamePost(a52, ca, 6, 14);
	m.addAgent(a52, sdn);

	Agent* a53 = new Agent("53", 125, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a53, ca, 0, 7);
	a53->setCalendarDay(ca, 30, true);
	m.addAgent(a53, sdn);

	Agent* a55 = new Agent("55", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a55->setCalendarDay(ca, 3, true);
	a55->setCalendarDay(cgh, 5, true);
	a55->setCalendarDay(ca, 10, true);
	a55->setCalendarDay(cgh, 12, true);
	a55->setCalendarDay(ca, 17, true);
	a55->setCalendarDay(cgh, 19, true);
	a55->setCalendarDay(ca, 24, true);
	a55->setCalendarDay(cgh, 26, true);
	addConsecutiveSamePost(a55, ca, 27, 29);
	m.addAgent(a55, sdn);

	Agent* a58 = new Agent("58", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a58, cs, 10, 12);
	addConsecutiveSamePost(a58, ca, 13, 21);
	m.addAgent(a58, sdn);

	Agent* a62 = new Agent("62", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a62, ca, 0, 2);
	addConsecutiveSamePost(a62, ca, 12, 13);
	m.addAgent(a62, sdn);

	Agent* a64 = new Agent("64", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a64, ca, 0, 1);
	m.addAgent(a64, sdn);

	Agent* a8 = new Agent("8", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a8, ca, 0, 30);
	m.addAgent(a8, sdn);

	Agent* a21 = new Agent("21", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a21, ca, 0, 30);
	m.addAgent(a21, sdn);

	Agent* a27 = new Agent("27", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a27, ca, 0, 30);
	m.addAgent(a27, sdn);

	Agent* a41 = new Agent("41", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a41, ca, 0, 30);
	m.addAgent(a41, sdn);

	Agent* a42 = new Agent("42", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a42, ca, 0, 30);
	m.addAgent(a42, sdn);

	Agent* a45 = new Agent("45", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a45->setCalendarDay(ca, 0, true);
	m.addAgent(a45, sdn);

	Agent* a60 = new Agent("60", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a60, sdn);

	Agent* a54 = new Agent("54", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a54->setCalendarDay(fp, 23, true);
	m.addAgent(a54, sdn);

	Agent* a37 = new Agent("37", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a37->setCalendarDay(fp, 23, true);
	m.addAgent(a37, sdn);

	return m;
}

Model HandWrittedModels::addServiceCSMarch20(Model m)
{
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
	Post* ca = new Post("Ca", 7.5, false);
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

	//Agents

	Agent* a31 = new Agent("31", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a31, diag, 1, 5);
	addConsecutiveSamePost(a31, diag, 8, 12);
	addConsecutiveSamePost(a31, diag, 15, 19);
	addConsecutiveSamePost(a31, diag, 22, 26);
	addConsecutiveSamePost(a31, diag, 29, 30);
	m.addAgent(a31, cs_service);

	Agent* a11 = new Agent("11", 78, nbHoursWeek, Status::Confirmed);
	a11->setCalendarDay(repos, 1, true);
	a11->setCalendarDay(cs, 2, true);
	a11->setCalendarDay(repos, 3, true);
	a11->setCalendarDay(repos, 8, true);
	a11->setCalendarDay(cs, 9, true);
	a11->setCalendarDay(repos, 10, true);
	a11->setCalendarDay(repos, 15, true);
	a11->setCalendarDay(cs, 16, true);
	a11->setCalendarDay(repos, 17, true);
	a11->setCalendarDay(repos, 22, true);
	a11->setCalendarDay(cs, 23, true);
	a11->setCalendarDay(repos, 24, true);
	a11->setCalendarDay(repos, 29, true);
	a11->setCalendarDay(cs, 30, true);
	m.addAgent(a11, cs_service);

	Agent* a13 = new Agent("13", nbHoursMonth, nbHoursWeek, Status::Confirmed);
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
	m.addAgent(a13, cs_service);

	Agent* a22 = new Agent("22", 78, nbHoursWeek, Status::Confirmed);
	a22->setCalendarDay(cgh, 1, true);
	a22->setCalendarDay(acu, 4, true);
	a22->setCalendarDay(cgh, 8, true);
	a22->setCalendarDay(acu, 11, true);
	addConsecutiveSamePost(a22, ca, 13, 14);
	a22->setCalendarDay(cgh, 15, true);
	a22->setCalendarDay(cgh, 22, true);
	a22->setCalendarDay(acu, 25, true);
	a22->setCalendarDay(cgh, 29, true);
	m.addAgent(a22, cs_service);

	Agent* a35 = new Agent("35", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a35, ca, 15, 19);
	m.addAgent(a35, cs_service);


	Agent* a44 = new Agent("44", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	auto ip = vector<Post*>();
	ip.push_back(orientation);
	a44->setImpossiblePosts(ip);
	addConsecutiveSamePost(a44, ca, 0, 3);
	addConsecutiveSamePost(a44, ca, 20, 21);
	a44->setCalendarDay(fp, 23, true);
	a44->setImpossiblePosts(ip);
	m.addAgent(a44, cs_service);

	return m;

}

Model HandWrittedModels::addServiceExploMarch20(Model m)
{
	Service* explo = new Service("Explo");

	m.addService(explo);

	Post* explo_fonctionnelle = new Post("+", 7.5);
	explo_fonctionnelle->addAttribut(0);
	explo_fonctionnelle->addAttribut(2);

	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);

	Post* ca = new Post("Ca", 7.5, false);
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

Model HandWrittedModels::addServicePoolMarch20(Model m)
{
	//Ajout du service Pool à un modèle existant

	Service* pool = new Service("Pool");

	m.addService(pool);

	//Posts
	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 7.5, false);
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

	//Si samedi Jg alors Jg dimanche + lundi
	auto v = vector<int>();
	v.push_back(3);
	auto v2 = vector<int>();
	v2.push_back(3);
	v2.push_back(3);
	ConstraintInvolved* cwjjj = new ConstraintInvolved(v, v2, 5, 1000);
	pool->addConstraint(cwjjj);

	//Si dimanche Jg alors Jg lundi
	v = vector<int>();
	v.push_back(3);
	v2 = vector<int>();
	v2.push_back(3);
	ConstraintInvolved* cwjj = new ConstraintInvolved(v, v2, 6, 1000);
	pool->addConstraint(cwjj);

	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	//Agents

	Agent* a16 = new Agent("16", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a16, cs, 3, 5);
	addConsecutiveSamePost(a16, ca, 13, 14);
	addConsecutiveSamePost(a16, ca, 26, 28);
	m.addAgent(a16, pool);

	Agent* a25 = new Agent("25", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a25, ca, 20, 30);
	m.addAgent(a25, pool);

	Agent* a47 = new Agent("47", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a47, fp, 10, 12);
	a47->setCalendarDay(cs, 22, true);
	a47->setCalendarDay(fp, 23, true);
	a47->setCalendarDay(cs, 24, true);
	m.addAgent(a47, pool);

	Agent* a18 = new Agent("18", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a18, ca, 10, 16);
	addConsecutiveSamePost(a18, cs, 17, 19);
	m.addAgent(a18, pool);

	return m;
}

Model HandWrittedModels::generateMarch20()
{
	Model m = generateGhrMarch20();

	m.getServices()[0]->loadPredefinedPlanning("data/prePlannings/GHR.csv", m.getNbDays(), m.getFirstDay());
	m = addServiceSDCMarch20(m);
	m.getServices()[1]->loadPredefinedPlanning("data/prePlannings/SDC.csv", m.getNbDays(), m.getFirstDay());
	m = addServiceSDNMarch20(m);
	m.getServices()[2]->loadPredefinedPlanning("data/prePlannings/SDN.csv", m.getNbDays(), m.getFirstDay());
	m = addServicePoolMarch20(m);
	m = addServiceCSMarch20(m);
	m = addServiceExploMarch20(m);
	m = addServiceUKMarch20(m);
	m.getServices()[6]->loadPredefinedPlanning("data/prePlannings/UK.csv", m.getNbDays(), m.getFirstDay());
	m.printPlanning();

	return m;
}

Model HandWrittedModels::generateGhrApril20()
{
	//Creation du Model pour le service GHR

	Model m = Model(2, 30, 25);

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

	Post* ca = new Post("Ca", 7.5, false);
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

	Post* cgh = new Post("CGH", 7.5);
	cgh->addAttribut(0);
	cgh->addAttribut(2);

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

	//Si dimanche Jg alors Jg lundi
	v = vector<int>();
	v.push_back(3);
	v2 = vector<int>();
	v2.push_back(3);
	ConstraintInvolved* cwjj = new ConstraintInvolved(v, v2, 6, 1000);
	ghr->addConstraint(cwjj);

	float nbHoursWeek = 60, nbHoursMonth = 155;

	//Agents
	Agent* a1 = new Agent("1", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a1->setCalendarDay(oe, 0, true);
	a1->setCalendarDay(fp, 1, true);
	a1->setCalendarDay(oe, 7, true);
	a1->setCalendarDay(oe, 14, true);
	a1->setCalendarDay(oe, 21, true);
	a1->setCalendarDay(oe, 28, true);
	m.addAgent(a1, ghr);


	Agent* a6 = new Agent("6", nbHoursMonth, nbHoursWeek, Status::Beginner);
	auto ip = vector<Post*>();
	ip.push_back(ng);
	a6->setImpossiblePosts(ip);
	m.addAgent(a6, ghr);

	Agent* a24 = new Agent("24", 125, nbHoursWeek, Status::Confirmed);
	a24->setCalendarDay(cs, 5, true);
	a24->setCalendarDay(cs, 7, true);
	addConsecutiveSamePost(a24, ca, 13, 18);
	m.addAgent(a24, ghr);

	Agent* a36 = new Agent("36", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a36, ghr);

	Agent* a40 = new Agent("40", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a40, ca, 17, 18);
	m.addAgent(a40, ghr);

	Agent* a49 = new Agent("49", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a49->setCalendarDay(fp, 0, true);
	a49->setCalendarDay(fp, 1, true);
	m.addAgent(a49, ghr);

	Agent* a55 = new Agent("55", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a55->setCalendarDay(repos, 0, true);
	a55->setCalendarDay(cgh, 2, true);
	a55->setCalendarDay(repos, 7, true);
	a55->setCalendarDay(cgh, 9, true);
	a55->setCalendarDay(repos, 14, true);
	a55->setCalendarDay(cgh, 16, true);
	a55->setCalendarDay(repos, 21, true);
	a55->setCalendarDay(cgh, 23, true);
	a55->setCalendarDay(repos, 28, true);
	m.addAgent(a55, ghr);

	Agent* a63 = new Agent("63", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a63, ca, 10, 13);
	m.addAgent(a63, ghr);

	return m;
}

Model HandWrittedModels::addServiceSDCApril20(Model m)
{
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
	Post* ca = new Post("Ca", 7.5, false);
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
	Agent* a5 = new Agent("5", nbHoursMonth, nbHoursWeek, Status::Beginner);
	addConsecutiveSamePost(a5, ca, 8, 12);
	a5->setImpossiblePosts(ip);
	m.addAgent(a5, sdc);

	Agent* a9 = new Agent("9", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a9, ca, 0, 4);
	m.addAgent(a9, sdc);


	Agent* a12 = new Agent("12", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a12, ca, 0, 4);
	a12->setCalendarDay(ca, 6, true);
	addConsecutiveSamePost(a12, ca, 19, 25);
	a12->setCalendarDay(ca, 27, true);
	m.addAgent(a12, sdc);

	Agent* a17 = new Agent("17", 78, nbHoursWeek, Status::Confirmed);
	a17->setCalendarDay(repos, 0, true);
	a17->setCalendarDay(repos, 2, true);
	a17->setCalendarDay(repos, 7, true);
	a17->setCalendarDay(ca, 8, true);
	a17->setCalendarDay(repos, 9, true);
	addConsecutiveSamePost(a17, ca, 10, 13);
	a17->setCalendarDay(repos, 14, true);
	a17->setCalendarDay(ca, 15, true);
	a17->setCalendarDay(repos, 16, true);
	a17->setCalendarDay(repos, 21, true);
	a17->setCalendarDay(repos, 23, true);
	a17->setCalendarDay(repos, 18, true);
	m.addAgent(a17, sdc);

	Agent* a28 = new Agent("28", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a28, ca, 5, 11);
	addConsecutiveSamePost(a28, fp, 19, 20);
	addConsecutiveSamePost(a28, ca, 24, 25);
	m.addAgent(a28, sdc);

	Agent* a30 = new Agent("30", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a30, ca, 16, 19);
	m.addAgent(a30, sdc);

	Agent* a45 = new Agent("45", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a45, sdc);

	Agent* a46 = new Agent("46", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a46, ca, 0, 5);
	m.addAgent(a46, sdc);

	Agent* a50 = new Agent("50", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a50, ca, 13, 18);
	m.addAgent(a50, sdc);

	Agent* a59 = new Agent("59", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a59, ca, 0, 2);
	a59->setCalendarDay(ca, 14);
	m.addAgent(a59, sdc);

	return m;
}

Model HandWrittedModels::addServiceUKApril20(Model m)
{
	Service* UKang = new Service("UK");

	m.addService(UKang);

	Post* uk = new Post("UK", 12.25);
	uk->addAttribut(1);
	uk->addAttribut(0);
	uk->addAttribut(2);

	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 7.5, false);
	ca->addAttribut(5);
	ca->addAttribut(6);

	UKang->addPost(uk);
	UKang->addPost(repos);
	UKang->addPost(ca);
	UKang->addPostRequired(uk, 1);

	//Contraintes
	m.addBasicConstraintsTo(UKang);

	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	Agent* a34 = new Agent("34", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a34, ca, 13, 18);
	m.addAgent(a34, UKang);

	Agent* a61 = new Agent("61", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a61, ca, 8, 11);
	m.addAgent(a61, UKang);

	return m;
}

Model HandWrittedModels::addServiceSDNApril20(Model m)
{
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
	Post* ca = new Post("Ca", 7.5, false);
	ca->addAttribut(5);
	ca->addAttribut(6);

	Post* fp = new Post("FP", 7.5);
	fp->addAttribut(0);
	fp->addAttribut(2);

	Post* vff = new Post("VFF", 7.5);
	vff->addAttribut(0);
	vff->addAttribut(2);

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
	ip.push_back(nb);

	//Agents
	Agent* a2 = new Agent("2", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a2, ca, 19, 29);
	m.addAgent(a2, sdn);

	Agent* a3 = new Agent("3", 125, nbHoursWeek, Status::Confirmed);
	a3->setCalendarDay(repos, 0, true);
	a3->setCalendarDay(ca, 5, true);
	a3->setCalendarDay(repos, 7, true);
	a3->setCalendarDay(ca, 13, true);
	a3->setCalendarDay(repos, 14, true);
	addConsecutiveSamePost(a3, ca, 15, 18);
	a3->setCalendarDay(repos, 21, true);
	a3->setCalendarDay(repos, 28, true);
	m.addAgent(a3, sdn);

	Agent* a4 = new Agent("4", 78, nbHoursWeek, Status::Confirmed);
	a4->setCalendarDay(repos, 2, true);
	a4->setCalendarDay(repos, 9, true);
	a4->setCalendarDay(repos, 16, true);
	a4->setCalendarDay(repos, 23, true);
	addConsecutiveSamePost(a4, ca, 24, 25);
	a4->setCalendarDay(repos, 28, true);
	m.addAgent(a4, sdn);


	Agent* a7 = new Agent("7", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a7, fp, 0, 1);
	addConsecutiveSamePost(a7, ca, 5, 13);
	m.addAgent(a7, sdn);

	Agent* a10 = new Agent("10", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a10, ca, 0, 2);
	a10->setCalendarDay(cs, 5, true);
	a10->setCalendarDay(cgh, 8, true);
	a10->setCalendarDay(cgh, 15, true);
	a10->setCalendarDay(cs, 19, true);
	a10->setCalendarDay(cgh, 22, true);
	a10->setCalendarDay(cs, 26, true);
	a10->setCalendarDay(ca, 28, true);
	a10->setCalendarDay(cgh, 29, true);
	m.addAgent(a10, sdn);

	Agent* a19 = new Agent("19", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a19->setCalendarDay(fp, 1, true);
	addConsecutiveSamePost(a19, ca, 3, 7);
	m.addAgent(a19, sdn);

	Agent* a20 = new Agent("20", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	// a20->setService(sdn);
	a20->setCalendarDay(cs, 0, true);
	a20->setCalendarDay(ort, 2, true);
	a20->setCalendarDay(ort, 6, true);
	addConsecutiveSamePost(a20, ca, 9, 16);
	a20->setCalendarDay(ort, 20, true);
	a20->setCalendarDay(ort, 23, true);
	a20->setCalendarDay(cs, 26, true);
	a20->setCalendarDay(ort, 27, true);
	a20->setCalendarDay(cs, 28, true);
	m.addAgent(a20, sdn);

	Agent* a23 = new Agent("23", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a23->setCalendarDay(eo, 1, true);
	a23->setCalendarDay(eo, 8, true);
	a23->setCalendarDay(eo, 15, true);
	a23->setCalendarDay(eo, 22, true);
	a23->setCalendarDay(eo, 29, true);
	m.addAgent(a23, sdn);

	Agent* a26 = new Agent("26", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a26, ca, 17, 18);
	a26->setCalendarDay(ca, 20, true);
	m.addAgent(a26, sdn);

	Agent* a32 = new Agent("32", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a32, sdn);

	Agent* a33 = new Agent("33", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a33, ca, 0, 4);
	a33->setCalendarDay(cs, 21, true);
	addConsecutiveSamePost(a33, vff, 22, 23);
	addConsecutiveSamePost(a33, ca, 24, 25);
	m.addAgent(a33, sdn);

	Agent* a38 = new Agent("38", 78, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a38, fp, 5, 8);
	a38->setCalendarDay(ca, 11, true);
	a38->setCalendarDay(cgh, 14, true);
	a38->setCalendarDay(cgh, 20, true);
	addConsecutiveSamePost(a38, ca, 23, 26);
	a38->setCalendarDay(cgh, 27, true);
	m.addAgent(a38, sdn);

	Agent* a39 = new Agent("39", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a39, ca, 1, 5);
	m.addAgent(a39, sdn);

	Agent* a43 = new Agent("43", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a43, ca, 0, 18);
	m.addAgent(a43, sdn);

	Agent* a51 = new Agent("51", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a51, sdn);

	Agent* a52 = new Agent("52", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a52, ca, 8, 9);
	addConsecutiveSamePost(a52, ca, 18, 22);
	addConsecutiveSamePost(a52, fp, 28, 29);
	m.addAgent(a52, sdn);

	Agent* a53 = new Agent("53", 125, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a53, ca, 0, 18);
	m.addAgent(a53, sdn);

	Agent* a56 = new Agent("56", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a56, ca, 2, 9);
	m.addAgent(a56, sdn);

	Agent* a57 = new Agent("57", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a57, ca, 0, 14);
	m.addAgent(a57, sdn);

	Agent* a58 = new Agent("58", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a58->setCalendarDay(cs, 7, true);
	addConsecutiveSamePost(a58, vff, 8, 9);
	a58->setCalendarDay(vff, 15, true);
	addConsecutiveSamePost(a58, ca, 28, 29);
	m.addAgent(a58, sdn);

	Agent* a64 = new Agent("64", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a64, ca, 15, 18);
	m.addAgent(a64, sdn);

	Agent* a8 = new Agent("8", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a8, ca, 0, 29);
	m.addAgent(a8, sdn);

	Agent* a15 = new Agent("15", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a15, ca, 0, 29);
	m.addAgent(a15, sdn);

	Agent* a21 = new Agent("21", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a21, ca, 0, 29);
	m.addAgent(a21, sdn);

	Agent* a27 = new Agent("27", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a27, ca, 0, 29);
	m.addAgent(a27, sdn);

	Agent* a41 = new Agent("41", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a41, ca, 0, 29);
	m.addAgent(a41, sdn);

	Agent* a42 = new Agent("42", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a42, ca, 0, 29);
	m.addAgent(a42, sdn);

	Agent* a60 = new Agent("60", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a60, ca, 3, 12);
	m.addAgent(a60, sdn);

	Agent* a48 = new Agent("48", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a48, ca, 15, 28);
	m.addAgent(a48, sdn);

	Agent* a54 = new Agent("54", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a54, ca, 2, 4);
	a54->setCalendarDay(ca, 14, true);
	a54->setCalendarDay(ca, 29, true);
	m.addAgent(a54, sdn);

	Agent* a37 = new Agent("37", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	m.addAgent(a37, sdn);

	Agent* a65 = new Agent("65", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a65->setImpossiblePosts(ip);
	m.addAgent(a65, sdn);

	Agent* vac1 = new Agent("vac1", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(vac1, repos, 0, 29);
	vac1->setCalendarDay(jb, 5, true);
	vac1->setCalendarDay(jb, 12, true);
	m.addAgent(vac1, sdn);

	Agent* vac2 = new Agent("vac2", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(vac2, repos, 0, 29);
	m.addAgent(vac2, sdn);

	return m;
}

Model HandWrittedModels::addServiceCSApril20(Model m)
{
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

	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 7.5, false);
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

	//Agents

	Agent* a31 = new Agent("31", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a31, diag, 0, 2);
	addConsecutiveSamePost(a31, diag, 5, 9);
	addConsecutiveSamePost(a31, diag, 13, 16);
	addConsecutiveSamePost(a31, diag, 19, 21);
	addConsecutiveSamePost(a31, ca, 22, 29);
	m.addAgent(a31, cs_service);

	Agent* a11 = new Agent("11", 78, nbHoursWeek, Status::Confirmed);
	a11->setCalendarDay(repos, 0, true);
	a11->setCalendarDay(repos, 5, true);
	a11->setCalendarDay(cs, 6, true);
	a11->setCalendarDay(repos, 7, true);
	a11->setCalendarDay(repos, 12, true);
	a11->setCalendarDay(cs, 3, true);
	a11->setCalendarDay(repos, 14, true);
	addConsecutiveSamePost(a11, ca, 15, 18);
	a11->setCalendarDay(repos, 19, true);
	a11->setCalendarDay(ca, 20, true);
	a11->setCalendarDay(repos, 21, true);
	addConsecutiveSamePost(a11, ca, 22, 25);
	a11->setCalendarDay(repos, 26, true);
	a11->setCalendarDay(cs, 27, true);
	a11->setCalendarDay(repos, 28, true);
	m.addAgent(a11, cs_service);

	Agent* a13 = new Agent("13", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a13->setCalendarDay(eo, 0, true);
	a13->setCalendarDay(cs, 5, true);
	a13->setCalendarDay(ban, 6, true);
	a13->setCalendarDay(eo, 7, true);
	a13->setCalendarDay(ban, 13, true);
	a13->setCalendarDay(eo, 14, true);
	a13->setCalendarDay(cs, 19, true);
	a13->setCalendarDay(ban, 20, true);
	a13->setCalendarDay(eo, 21, true);
	a13->setCalendarDay(cs, 26, true);
	a13->setCalendarDay(ban, 27, true);
	a13->setCalendarDay(eo, 28, true);
	m.addAgent(a13, cs_service);

	Agent* a22 = new Agent("22", 78, nbHoursWeek, Status::Confirmed);
	a22->setCalendarDay(acu, 1, true);
	a22->setCalendarDay(acu, 15, true);
	a22->setCalendarDay(cgh, 19, true);
	a22->setCalendarDay(acu, 22, true);
	a22->setCalendarDay(cgh, 26, true);
	a22->setCalendarDay(acu, 29, true);
	m.addAgent(a22, cs_service);

	Agent* a35 = new Agent("35", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a35, orientation, 1, 2);
	addConsecutiveSamePost(a35, orientation, 5, 6);
	a35->setCalendarDay(orientation, 8, true);
	a35->setCalendarDay(ca, 9, true);
	a35->setCalendarDay(orientation, 13, true);
	addConsecutiveSamePost(a35, orientation, 15, 16);
	addConsecutiveSamePost(a35, fp, 19, 21);
	addConsecutiveSamePost(a35, orientation, 22, 23);
	addConsecutiveSamePost(a35, orientation, 26, 27);
	a35->setCalendarDay(orientation, 29, true);
	m.addAgent(a35, cs_service);


	Agent* a14 = new Agent("14", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	auto ip = vector<Post*>();
	ip.push_back(orientation);
	a14->setImpossiblePosts(ip);
	a14->setCalendarDay(ban, 6, true);
	addConsecutiveSamePost(a14, ban, 8, 9);
	addConsecutiveSamePost(a14, ban, 15, 16);
	a14->setCalendarDay(ban, 19, true);
	addConsecutiveSamePost(a14, ban, 22, 23);
	a14->setCalendarDay(ban, 26, true);
	a14->setCalendarDay(ban, 29, true);
	m.addAgent(a14, cs_service);

	return m;
}

Model HandWrittedModels::addServiceExploApril20(Model m)
{
	Service* explo = new Service("Explo");

	m.addService(explo);

	Post* explo_fonctionnelle = new Post("+", 7.5);
	explo_fonctionnelle->addAttribut(0);
	explo_fonctionnelle->addAttribut(2);

	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);

	Post* ca = new Post("Ca", 7.5, false);
	ca->addAttribut(5);
	ca->addAttribut(6);

	Post* fp = new Post("FP", 7.5);
	fp->addAttribut(0);
	fp->addAttribut(2);

	explo->addPost(explo_fonctionnelle);
	explo->addPost(repos);
	explo->addPost(fp);

	explo->addPostRequired(explo_fonctionnelle, 2, 0);
	explo->addPostRequired(explo_fonctionnelle, 2, 1);
	explo->addPostRequired(explo_fonctionnelle, 2, 2);
	explo->addPostRequired(explo_fonctionnelle, 2, 3);
	explo->addPostRequired(explo_fonctionnelle, 2, 4);

	m.addBasicConstraintsTo(explo);


	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	Agent* a43 = new Agent("43", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a43->setCalendarDay(fp, 1, true);
	addConsecutiveSamePost(a43, ca, 8, 11);
	addConsecutiveSamePost(a43, ca, 28, 29);
	m.addAgent(a43, explo);

	Agent* a62 = new Agent("62", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a62, ca, 2, 5);
	m.addAgent(a62, explo);

	return m;
}

Model HandWrittedModels::addServicePoolApril20(Model m)
{
	//Ajout du service Pool à un modèle existant

	Service* pool = new Service("Pool");

	m.addService(pool);

	//Posts
	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);
	Post* ca = new Post("Ca", 7.5, false);
	ca->addAttribut(5);
	ca->addAttribut(6);

	Post* vff = new Post("VFF", 7.5);
	vff->addAttribut(0);
	vff->addAttribut(2);

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

	//Si samedi Jg alors Jg dimanche + lundi
	auto v = vector<int>();
	v.push_back(3);
	auto v2 = vector<int>();
	v2.push_back(3);
	v2.push_back(3);
	ConstraintInvolved* cwjjj = new ConstraintInvolved(v, v2, 5, 1000);
	pool->addConstraint(cwjjj);

	//Si dimanche Jg alors Jg lundi
	v = vector<int>();
	v.push_back(3);
	v2 = vector<int>();
	v2.push_back(3);
	ConstraintInvolved* cwjj = new ConstraintInvolved(v, v2, 6, 1000);
	pool->addConstraint(cwjj);

	float nbHoursWeek = 60.0, nbHoursMonth = 155;

	//Agents

	Agent* a16 = new Agent("16", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a16, ca, 0, 3);
	addConsecutiveSamePost(a16, ca, 10, 12);
	addConsecutiveSamePost(a16, ca, 19, 21);
	a16->setCalendarDay(cs, 28, true);
	a16->setCalendarDay(vff, 29, true);
	m.addAgent(a16, pool);

	Agent* a25 = new Agent("25", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a25, ca, 0, 3);
	addConsecutiveSamePost(a25, ca, 24, 25);
	m.addAgent(a25, pool);

	Agent* a47 = new Agent("47", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	addConsecutiveSamePost(a47, ca, 10, 12);
	addConsecutiveSamePost(a47, ca, 19, 22);
	m.addAgent(a47, pool);

	Agent* a18 = new Agent("18", nbHoursMonth, nbHoursWeek, Status::Confirmed);
	a18->setCalendarDay(ca, 4, true);
	addConsecutiveSamePost(a18, fp, 5, 8);
	a18->setCalendarDay(cs, 14, true);
	addConsecutiveSamePost(a18, ca, 15, 16);
	addConsecutiveSamePost(a18, ca, 19, 21);
	m.addAgent(a18, pool);

	return m;
}

Model HandWrittedModels::generateApril20()
{
	Model m = generateGhrApril20();

	m.getServices()[0]->loadPredefinedPlanning("data/prePlannings/GHR.csv", m.getNbDays(), m.getFirstDay());
	m = addServiceSDCApril20(m);
	m.getServices()[1]->loadPredefinedPlanning("data/prePlannings/SDC.csv", m.getNbDays(), m.getFirstDay());
	m = addServiceSDNApril20(m);
	m.getServices()[2]->loadPredefinedPlanning("data/prePlannings/SDN.csv", m.getNbDays(), m.getFirstDay());
	m = addServicePoolApril20(m);
	m = addServiceCSApril20(m);
	m = addServiceExploApril20(m);
	m = addServiceUKApril20(m);
	m.getServices()[6]->loadPredefinedPlanning("data/prePlannings/UK.csv", m.getNbDays(), m.getFirstDay());
	m.printPlanning();

	return m;
}
