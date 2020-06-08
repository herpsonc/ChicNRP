#include "LPSolver.h"
#include "VariableData.h"
//using namespace std;

LPSolver::LPSolver() {
	// TODO Auto-generated constructor stub
}


LPSolver::~LPSolver() {
	// TODO Auto-generated destructor stub
}


Model LPSolver::linearProgram(const Model mo) {
	Model mr = Model(mo);

	int n_tmp = 0, m_tmp = 0;
	cout << "nb jours du mois : " << mr.getNbDays() << endl;
	for (auto s : mr.getServices()) {
		m_tmp += mr.getAgentFrom(s).size(); //m : nb_agents (=nb_vars pour un jour en fonction d'un poste)
		cout << "Service " << s->getId() << ": " << endl;

		cout << "nb agents: " << mr.getAgentFrom(s).size() << endl;

		cout << "nb postes : " << s->getPosts().size() << endl;

		for (int day = 0; day < mr.getNbDays(); day++) {
			n_tmp += s->getPosts().size(); //n : nb_contraintes
		}
	}


	SCIP* scip;
	SCIPcreate(&scip);

	SCIPincludeDefaultPlugins(scip);
	// SCIPmessagehdlrSetQuiet(SCIPgetMessagehdlr(scip), TRUE);
	// uncomment the above line to disable output

	SCIPcreateProb(scip, "planning", NULL, NULL, NULL, NULL, NULL, NULL, NULL); //sens par défaut : Min

	// Liste contraintes
	vector<SCIP_CONS*> consVec = vector<SCIP_CONS*>();
	// Liste variables
	vector<VariableData*> varsData = vector<VariableData*>(); //contient les variables SCIP + données liées (id, agent, jour, poste)

	ofstream fileIdVar("idvar.txt");

	int id_contrainte = 0;
	int id_var = 0; //agent

	vector<Agent*> agents;


	for (int day = 0; day < mr.getNbDays(); day++) {

		for (auto s : mr.getServices()) {
			cout << "Service " << s->getId() << endl;
			//défaut : ne prend pas en compte les agents qu'on peut solliciter en dehors du service où ce poste est dédié en général
			agents = mr.getAgentFrom(s);

			for (auto job : s->getPosts()) { //pour généraliser : mr.getPosts() mais il faudrait d'autres changements
				if (job->getId() != "Repos") {

					// Liste variables contrainte courante
					int v = 0;
					SCIP_VAR* varCons[50];

					bool agent_dispo = false;
					for (auto a : agents) {

						SCIP_VAR* var;
						if (a->getCalendar()[day] == NULL) {
							if (!agent_dispo) { //permet d'ajouter une contrainte seulement s'il y a au moins un agent dispo pour ce job à cette date
								agent_dispo = true;
							}
							//cout << "X_a" << a->getId() << ",j" << day+1 << "," << job->getId() << "= x(" << id_var << ")" << endl;
							fileIdVar << "X_a" << a->getId() << ",j" << day + 1 << "," << job->getId() << "= _var" << id_var << "_" << endl;

							//job->getTime() possible au lieu de 1.0 pour le coefficient des vars dans la fonction objectif
							SCIPcreateVar(scip, &var, NULL, 0.0, 1.0, job->getTime(), SCIP_VARTYPE_BINARY, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL);
							SCIPaddVar(scip, var);

							varsData.push_back(new VariableData(var, id_var, day, a, job));

							varCons[v] = var;
							v++;

							id_var++;
						}
						/*else {
							fileIdVar << "X_a" << a->getId() << ",j" << day + 1 << "," << job->getId() << "= _var" << id_var << "_" << endl;

							//job->getTime() possible au lieu de 1.0 pour le coefficient des vars dans la fonction objectif
							SCIPcreateVar(scip, &var, NULL, 1.0, 1.0, 1.0, SCIP_VARTYPE_BINARY, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL);
							SCIPaddVar(scip, var);

							varsData.push_back(new VariableData(var, id_var, day, a, a->getCalendar()[day]));

							varCons[v] = var;
							v++;

							id_var++;
						}*/

					}

					if (agent_dispo) {
						// Constraint = 1
						SCIP_CONS* cons;
						SCIPcreateConsBasicSetpart(scip, &cons, "agent_dispo", v, varCons);
						SCIPaddCons(scip, cons);

						consVec.push_back(cons);

						//SCIPprintCons(scip, cons, NULL);
					}

					id_contrainte++;
				}
			}


			//un agent ne peut faire qu'un poste par jour
			vector<VariableData*> varsDataTmp(varsData); //variable temporaire pour la contrainte
			for (auto a : agents) {
				if (a->getCalendar()[day] == NULL) {

					// Liste variables contrainte courante
					int v = 0;
					SCIP_VAR* varCons[50];

					for (auto job : s->getPosts()) {
						if (job->getId() != "Repos") {
							for (int i = 0; i < varsDataTmp.size(); i++) {
								if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == day && varsDataTmp[i]->getPost() == job) {
									varCons[v] = varsDataTmp[i]->getVariable();
									varsDataTmp.erase(varsDataTmp.begin() + i); //pour optimiser les prochaines recherches de variables
									v++;
								}
							}
						}
					}

					//Création contrainte <= 1
					SCIP_CONS* cons;
					SCIPcreateConsBasicSetpack(scip, &cons, "un_poste_par_jour", v, varCons);
					SCIPaddCons(scip, cons);

					// Ajout liste contraintes
					consVec.push_back(cons);

					id_contrainte++;
				}
			}
		}

	}

	/*
	int it = 0;
	int* tab;
	vector<VariableData*> varsDataTmp(varsData); //pour ne pas re-avoir les agents déjà vus
	for (int i = 0; i < agents.size(); i++) {
		cout << "agent:" << agents[i]->getId() << endl;
		for (int i = 0; i < varsDataTmp.size(); i++) {
			if (agents[i] == varsDataTmp[i]->getAgent()) {
				it++;
				varsDataTmp.erase(varsDataTmp.begin()+i);
			
			}
		}
		tab[i] = { it };
	}

	const int n = a;
	*/


	/*Contraintes horaires par Mois */

	vector<VariableData*> varsDataTmp(varsData); //sert à ne pas re-avoir les agents déjà vus
	int nb_vals = 0;
	for (auto a : agents) {
		nb_vals = 0;
		long long coeffs_hours[200];  //taille de tableau en dur à changer dès que possible
		SCIP_VAR* varsConsHours[200]; //maximum 31*nbPosts vars par mois
		cout << "tabl 10 : " << coeffs_hours[10] << endl;
		cout << "agent:" << a->getId() << endl;
		for (int i = 0; i < varsDataTmp.size(); i++) {
			if (a->getId() == varsDataTmp[i]->getAgent()->getId()){ //&& varsDataTmp[i]->getVariable() >= 0) {
				coeffs_hours[nb_vals] = static_cast<long long>(varsDataTmp[i]->getPost()->getTime());
				varsConsHours[nb_vals] = varsDataTmp[i]->getVariable();

				cout << "id: " << varsDataTmp[i]->getId() << "  " << varsDataTmp[i]->getPost()->getId() << varsDataTmp[i]->getDay() << " heures " << coeffs_hours[nb_vals] << endl;
				//varsDataTmp.erase(varsDataTmp.begin()+i); //à l'air de faire buguer la contrainte
				nb_vals++;
			}
		}

		cout << "fin agent:" << a->getId() << " " << nb_vals << "  " << sizeof(coeffs_hours)/ sizeof(coeffs_hours[0]) << "  " << sizeof(varsConsHours)/ sizeof(varsConsHours[0]) << endl;

		SCIP_CONS* consHoursMonth;
		SCIPcreateConsBasicKnapsack(scip, &consHoursMonth, "nb_heures_mois", nb_vals, varsConsHours, coeffs_hours, a->getNbHoursMonth());//+mo.getOvertime());

		//modifier la contrainte pour la créer plutôt avec une contrainte linéaire :
		//SCIPcreateConsLinear(scip, &consHoursMonth, NULL, nb_vals, varsConsHours, coeffs_hours, a->getNbHoursMonth(), NULL,);
		//SCIPaddCoefLinear(scip, consHoursMonth, varsConsHours[0], a->getNbHoursMonth());

		SCIPaddCons(scip, consHoursMonth);

		cout << "contrainte ok" << endl;
		consVec.push_back(consHoursMonth);
	}


	/*Contraintes horaires par Semaine */

	varsDataTmp = varsData; //pour ne pas re-avoir les agents déjà vus
	nb_vals = 0;
	int jour_semaine = 1; 
	for (auto a : agents) {
		jour_semaine = 1;
		nb_vals = 0;
		long long coeffs_hours[200];
		SCIP_VAR* varsConsHours[200]; //maximum 7*nbPosts vars par semaine
		for (int day = 0; day < mr.getNbDays(); day++) {

			//cout << "agent:" << a->getId() << endl;
			for (int i = 0; i < varsDataTmp.size(); i++) {
				if (a->getId() == varsDataTmp[i]->getAgent()->getId() && varsDataTmp[i]->getDay() == day) {
					coeffs_hours[nb_vals] = static_cast<long long>(varsDataTmp[i]->getPost()->getTime());
					varsConsHours[nb_vals] = varsDataTmp[i]->getVariable();

					//cout << "id: " << varsDataTmp[i]->getId() << "  " << varsDataTmp[i]->getPost()->getId() << varsDataTmp[i]->getDay() << " heures " << coeffs_hours[nb_vals] << endl;
					//varsDataTmp.erase(varsDataTmp.begin()+i); //à l'air de faire buguer la contrainte
					nb_vals++;
				}
			}

			//cout << "fin agent:" << a->getId() << " " << nb_vals << "  " << sizeof(coeffs_hours) / sizeof(coeffs_hours[0]) << "  " << sizeof(varsConsHours) / sizeof(varsConsHours[0]) << endl;


			if (jour_semaine == 7 || day == mr.getNbDays()-1) {
				SCIP_CONS* consHoursWeek;
				SCIPcreateConsBasicKnapsack(scip, &consHoursWeek, "nb_heures_semaine", nb_vals, varsConsHours, coeffs_hours, a->getNbHoursWeek());//+mo.getOvertime());
				SCIPaddCons(scip, consHoursWeek);

				cout << "contrainte ok" << endl;
				consVec.push_back(consHoursWeek);
				jour_semaine = 1;
				nb_vals = 0;
				long long coeffs_hours[200];
				SCIP_VAR* varsConsHours[200]; //maximum 7*nbPosts vars par semaine
			}
			jour_semaine++;
		}
	}

	// Nuit puis Jour interdit
	varsDataTmp = varsData; //variable temporaire pour la contrainte
	for (int day = 0; day < mr.getNbDays(); day++) {
		for (auto s : mr.getServices()) {
			if (day < mr.getNbDays() - 1) {
				for (auto a : agents) {

					// Vérifier si l'agent ne travaille pas déjà de jour le jour suivant
					bool workByDayAfter = false;
					int d1 = day + 1;

					auto work1 = a->getCalendar()[d1];
					if (work1 != NULL) {
						vector<string> att = work1->getAttributs();
						if (count(att.begin(), att.end(), "day")) {
							workByDayAfter = true;
						}
					}

					if (a->getCalendar()[day] == NULL || workByDayAfter) {
						for (auto nightJob : s->getPosts()) {

							vector<string> attNight = nightJob->getAttributs();

							if (count(attNight.begin(), attNight.end(), "night")) {

								int v = 0;
								SCIP_VAR* varCons[10];
								SCIP_VAR* varNight;
								for (int i = 0; i < varsDataTmp.size(); i++) {
									if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == day && varsDataTmp[i]->getPost() == nightJob) {
										varNight = varsDataTmp[i]->getVariable();
										varCons[v] = varNight;
										v++;
										break;
									}
								}

								// Jour suivant
								for (auto dayJob : s->getPosts()) {
									vector<string> attDay = dayJob->getAttributs();

									if (count(attDay.begin(), attDay.end(), "day")) {

										SCIP_VAR* varDay;
										bool noVar = true;

										for (int i = 0; i < varsDataTmp.size(); i++) {
											// Poste jour suivant == NULL
											if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == (day + 1) && varsDataTmp[i]->getPost() == dayJob && v == 1) {
												varDay = varsDataTmp[i]->getVariable();
												varCons[v] = varDay;
												v++;
												//cout << v << endl;
												SCIP_CONS* cons;
												SCIPcreateConsBasicSetpack(scip, &cons, "nuit_jour_interdit", v, varCons);
												SCIPaddCons(scip, cons);

												auto it = consVec.end();
												consVec.insert(it, cons);

												id_contrainte++;
												noVar = false;

												//Break si variable trouvée
												break;
											}
										}

										// Poste jour suivant != NULL (il n'existe pas de variable)
										if (noVar && v == 1) {
											SCIP_CONS* cons;
											long long un[1];
											un[0] = (long long)1;
											SCIPcreateConsBasicKnapsack(scip, &cons, "nuit_jour_interdit", v, varCons, un, 0);
											SCIPaddCons(scip, cons);

											auto it = consVec.end();
											consVec.insert(it, cons);

											id_contrainte++;

											//Break car contrainte finie pour nightJob et DayJob courant
											break;
										}
									}
								}
							}
						}
					}
				}
			}

			// 3 nuits d'affilée interdit
			if (day < mr.getNbDays() - 2) {
				for (auto a : agents) {
					if (a->getCalendar()[day] == NULL) {
						int nbVar = 0;
						SCIP_VAR* varCons[10];
						for (auto nightOne : s->getPosts()) {

							vector<string> attNightOne = nightOne->getAttributs();

							if (count(attNightOne.begin(), attNightOne.end(), "night")) {

								SCIP_VAR* varNightOne;

								for (int i = 0; i < varsDataTmp.size(); i++) {
									if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == day && varsDataTmp[i]->getPost() == nightOne) {
										//cout << varsDataTmp[i]->getDay() << " " << varsDataTmp[i]->getAgent()->getId() << endl;
										varNightOne = varsDataTmp[i]->getVariable();
										varCons[nbVar] = varNightOne;
										nbVar++;
										break;
									}
								}
								int d1 = day + 1;
								if (a->getCalendar()[d1] == NULL) {
									for (auto nightTwo : s->getPosts()) {

										vector<string> attNightTwo = nightTwo->getAttributs();

										if (count(attNightTwo.begin(), attNightTwo.end(), "night")) {
											SCIP_VAR* varNightTwo;

											for (int i = 0; i < varsDataTmp.size(); i++) {
												if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == d1 && varsDataTmp[i]->getPost() == nightTwo) {
													//cout << varsDataTmp[i]->getDay() << " " << varsDataTmp[i]->getAgent()->getId() << endl;
													varNightTwo = varsDataTmp[i]->getVariable();
													varCons[nbVar] = varNightTwo;
													nbVar++;
													break;
												}
											}

											int d2 = d1 + 1;
											if (a->getCalendar()[d2] == NULL) {
												for (auto nightThree : s->getPosts()) {

													vector<string> attNightThree = nightThree->getAttributs();

													if (count(attNightThree.begin(), attNightThree.end(), "night")) {
														SCIP_VAR* varNightThree;

														for (int i = 0; i < varsDataTmp.size(); i++) {
															if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == d2 && varsDataTmp[i]->getPost() == nightThree) {
																//cout << varsDataTmp[i]->getDay() << " " << varsDataTmp[i]->getAgent()->getId() << endl;
																varNightThree = varsDataTmp[i]->getVariable();
																varCons[nbVar] = varNightThree;
																nbVar++;
																break;
															}
														}

														SCIP_CONS* cons;
														long long un[3];
														for (int i = 0; i < 3; i++) {
															un[i] = (long long)1;
														}

														SCIPcreateConsBasicKnapsack(scip, &cons, "trois_nuits_interdit", nbVar, varCons, un, 2);
														SCIPaddCons(scip, cons);

														auto it = consVec.end();
														consVec.insert(it, cons);

														id_contrainte++;

														//Break car contrainte finie pour day, day+1, day+2
														break;

													}
												}
											}
										}
									}
								}

							}
						}
					}
				}
			}

			// 3 jours d'affilée possible en GHR, SDC et UK
			string idService = s->getId();
			if (idService == "GHR" || idService == "SDC" || idService == "UK") {
				if (day < mr.getNbDays() - 3 && ((day + 1) % 4 == 0 || day == 0)) {
					for (auto a : agents) {
						for (auto dayOne : s->getPosts()) {
							int nbVar = 0;
							SCIP_VAR* varCons[10];

							if (nbVar >= 3) {
								nbVar = 0;
								SCIP_VAR* varCons[10];
							}
							vector<string> attDayOne = dayOne->getAttributs();

							if (count(attDayOne.begin(), attDayOne.end(), "day")) {
								SCIP_VAR* varDayOne;
								for (int i = 0; i < varsDataTmp.size(); i++) {
									if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == day && varsDataTmp[i]->getPost() == dayOne) {
										//cout << varsDataTmp[i]->getDay() << " " << varsDataTmp[i]->getAgent()->getId() << " " << varsDataTmp[i]->getPost()->getId() << endl;
										varDayOne = varsDataTmp[i]->getVariable();
										varCons[nbVar] = varDayOne;
										nbVar++;
										break;
									}
								}

								int d1 = day + 1;
								if (a->getCalendar()[d1] == NULL) {
									for (auto dayTwo : s->getPosts()) {

										vector<string> attDayTwo = dayTwo->getAttributs();

										if (count(attDayTwo.begin(), attDayTwo.end(), "day")) {
											SCIP_VAR* varDayTwo;

											if (nbVar >= 4) {
												nbVar = 1;
											}

											for (int i = 0; i < varsDataTmp.size(); i++) {
												if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == d1 && varsDataTmp[i]->getPost() == dayTwo) {
													//cout << varsDataTmp[i]->getDay() << " " << varsDataTmp[i]->getAgent()->getId() << " " << varsDataTmp[i]->getPost()->getId() << endl;
													varDayTwo = varsDataTmp[i]->getVariable();
													varCons[nbVar] = varDayTwo;
													nbVar++;
													break;
												}
											}

											int d2 = d1 + 1;
											if (a->getCalendar()[d2] == NULL) {
												for (auto dayThree : s->getPosts()) {

													vector<string> attDayThree = dayThree->getAttributs();

													if (count(attDayThree.begin(), attDayThree.end(), "day")) {
														SCIP_VAR* varDayThree;

														if (nbVar >= 4) {
															nbVar = 2;
														}

														for (int i = 0; i < varsDataTmp.size(); i++) {
															if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == d2 && varsDataTmp[i]->getPost() == dayThree) {
																//cout << varsDataTmp[i]->getDay() << " " << varsDataTmp[i]->getAgent()->getId() << " " << varsDataTmp[i]->getPost()->getId() << endl;
																varDayThree = varsDataTmp[i]->getVariable();
																varCons[nbVar] = varDayThree;
																nbVar++;
																break;
															}
														}
														//cout << a->getId() << " " << day << " " << dayOne->getId() << " " << dayTwo->getId() << " " << dayThree->getId() << " " << nbVar << endl;

														int d3 = d2 + 1;
														if (a->getCalendar()[d3] == NULL) {
															for (auto dayFour : s->getPosts()) {

																vector<string> attDayFour = dayFour->getAttributs();

																if (count(attDayFour.begin(), attDayFour.end(), "day")) {
																	SCIP_VAR* varDayFour;

																	if (nbVar >= 4) {
																		nbVar = 3;
																	}

																	for (int i = 0; i < varsDataTmp.size(); i++) {
																		if (varsDataTmp[i]->getAgent() == a && varsDataTmp[i]->getDay() == d3 && varsDataTmp[i]->getPost() == dayFour) {
																			//cout << varsDataTmp[i]->getDay() << " " << varsDataTmp[i]->getAgent()->getId() << " " << varsDataTmp[i]->getPost()->getId() << endl;
																			varDayFour = varsDataTmp[i]->getVariable();
																			varCons[nbVar] = varDayFour;
																			nbVar++;
																			break;
																		}
																	}

																	if (nbVar == 4) {
																		SCIP_CONS* cons;
																		long long un[4];
																		for (int i = 0; i < 4; i++) {
																			un[i] = (long long)1;
																		}


																		SCIPcreateConsBasicKnapsack(scip, &cons, "trois_jours_exceptionnel", nbVar, varCons, un, 3);
																		SCIPaddCons(scip, cons);

																		auto it = consVec.end();
																		consVec.insert(it, cons);

																		id_contrainte++;

																		//Break car contrainte finie pour day, day+1, day+2
																		//break;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}


	fileIdVar.close();

	//stocke les données du PL dans un fichier de log
	cout << "Write init pl" << endl;
	SCIPwriteOrigProblem(scip, "init2.txt", "lp", FALSE);

	SCIPsolve(scip);
	SCIP_SOL* sol = SCIPgetBestSol(scip);


	if (sol == NULL)
	{
		cout << "Solution = NULL" << endl;
	}

	//SCIPprintSol(scip, sol, NULL, TRUE);

	// Release & free +
	// Changer les calendriers des agents
	SCIP_VAR* var_tmp;
	for (auto varData : varsData) {
		SCIP_Real varValue = SCIPgetSolVal(scip, sol, varData->getVariable());
		if (varValue == 1.0) {
			//cout << "we good" << " id: " << id_var << " ag: " << agentsIndex[id_var]->getId() << " j: " << daysIndex[id_var]+1 << " p: " << postsIndex[id_var]->getId() << endl;
			varData->getAgent()->setCalendarDay(varData->getPost(), varData->getDay());
		}
		else if(varValue == .0 && varData->getAgent()->getCalendar()[varData->getDay()] == NULL){ //si on a pas un autre poste attribué plus tôt pour le même jour et le même agent ==> repos
			//cout << "not good " << id_var << endl;
			varData->getAgent()->setCalendarDay(new Post("Repos", 0.0), varData->getDay());
		}

		var_tmp = varData->getVariable();
		SCIPreleaseVar(scip, &var_tmp);
	}

	for (auto c : consVec) {
		SCIPreleaseCons(scip, &c);
	}
	SCIPfree(&scip);

	mr.printPlanning();

	return mr;

}
