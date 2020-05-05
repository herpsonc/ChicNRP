#include "LPSolver.h"
#include "VariableData.h"
//using namespace std;

LPSolver::LPSolver() {
	// TODO Auto-generated constructor stub
}


LPSolver::~LPSolver() {
	// TODO Auto-generated destructor stub
}


Model LPSolver::linearProgramTest(const Model mo) {
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

			//défaut : prend pas en compte les agents qu'on peut solliciter en dehors du service où ce poste est dédié en général
			agents = mr.getAgentFrom(s);

			for (auto job : s->getPosts()) {
				if (job->getId() != "Repos") {

					// Liste variables contrainte courante
					int v = 0;
					SCIP_VAR* varCons[50];

					bool agent_dispo = false;
					for (auto a : agents) {

						SCIP_VAR* var;
						if (a->getCalendar()[day] == NULL ) {
							if (!agent_dispo) { //permet d'ajouter une contrainte seulement s'il y a au moins un agent dispo pour ce job à cette date
								agent_dispo = true;
							}
							//cout << "X_a" << a->getId() << ",j" << day+1 << "," << job->getId() << "= x(" << id_var << ")" << endl;
							fileIdVar << "X_a" << a->getId() << ",j" << day + 1 << "," << job->getId() << "= _var" << id_var << "_" << endl;

							SCIPcreateVar(scip, &var, NULL, 0.0, 1.0, 1.0, SCIP_VARTYPE_BINARY, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL);
							SCIPaddVar(scip, var);

							varsData.push_back(new VariableData(var, id_var, day, a, job));

							varCons[v] = var;
							v++;

							id_var++;
						}

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

	fileIdVar.close();

	cout << "Write init pl" << endl;
	SCIPwriteOrigProblem(scip, "init3.txt", "lp", FALSE);

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
	for (int id_var = 0; id_var < varsData.size(); id_var++) {
		SCIP_Real varValue = SCIPgetSolVal(scip, sol, varsData[id_var]->getVariable());
		if (varValue == 1.0) {
			//cout << "we good" << " id: " << id_var << " ag: " << agentsIndex[id_var]->getId() << " j: " << daysIndex[id_var]+1 << " p: " << postsIndex[id_var]->getId() << endl;
			varsData[id_var]->getAgent()->setCalendarDay(varsData[id_var]->getPost(), varsData[id_var]->getDay());
		}
		else if(varValue == .0 && varsData[id_var]->getAgent()->getCalendar()[varsData[id_var]->getDay()] == NULL){ //si on a pas un autre poste attribué plus tôt pour le même jour et le même agent ==> repos
			//cout << "not good " << id_var << endl;
			varsData[id_var]->getAgent()->setCalendarDay(new Post("Repos", 0.0), varsData[id_var]->getDay());
		}

		var_tmp = varsData[id_var]->getVariable();
		SCIPreleaseVar(scip, &var_tmp);
	}

	for (auto c : consVec) {
		SCIPreleaseCons(scip, &c);
	}
	SCIPfree(&scip);

	mr.printPlanning();

	return mr;

}
