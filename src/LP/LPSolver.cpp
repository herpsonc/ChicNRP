#include "LPSolver.h"
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

	const int n = n_tmp, m = m_tmp, nz = n_tmp * m_tmp;

	/*try {*/
	SCIP* scip;
	SCIPcreate(&scip);

	SCIPincludeDefaultPlugins(scip);
	// SCIPmessagehdlrSetQuiet(SCIPgetMessagehdlr(scip), TRUE);
	// uncomment the above line to disable output

	SCIPcreateProb(scip, "planning", NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	// The default objective sense for SCIP problems is minimizing.

	// Liste contraintes
	vector<SCIP_CONS*> consVec = vector<SCIP_CONS*>();
	// Liste variables
	vector<SCIP_VAR*> varVec = vector<SCIP_VAR*>();

	map<string, SCIP_VAR*> vars;

	ofstream fileIdVar("idvar.txt");

	//tableaux qui serviront à reconstituer les données équivalentes à chaque variable
	vector<Agent*> agentsIndex = vector<Agent*>();
	vector<Post*> postsIndex = vector<Post*>();
	vector<int> daysIndex = vector<int>();

	int id_contrainte = 0;
	int id_var = 0; //agent

	vector<Agent*> agents = vector<Agent*>();

	// Nombre d'agents pour un poste
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
						if (a->getCalendar()[day] == NULL) {
							if (!agent_dispo) { //permet d'ajouter une contrainte seulement s'il y a au moins un agent dispo pour ce job à cette date
								agent_dispo = true;
							}
							//cout << "X_a" << a->getId() << ",j" << day+1 << "," << job->getId() << "= x(" << id_var << ")" << endl;
							fileIdVar << "X_a" << a->getId() << ",j" << day + 1 << "," << job->getId() << "= x(" << id_var << ")" << endl;


							SCIP_VAR* var;

							SCIPcreateVar(scip, &var, NULL, 0.0, 1.0, 1.0, SCIP_VARTYPE_BINARY, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL);
							SCIPaddVar(scip, var);

							agentsIndex.push_back(a);
							postsIndex.push_back(job);
							daysIndex.push_back(day);

							vars[a->getId() + "_" + to_string(day + 1) + "_" + job->getId()] = var;

							auto it = varVec.end();
							varVec.insert(it, var);

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

						auto it = consVec.end();
						consVec.insert(it, cons);

						//SCIPprintCons(scip, cons, NULL);
					}

					id_contrainte++;
				}
			}

			//un agent ne peut faire qu'un poste par jour
			for (auto a : agents) {
				if (a->getCalendar()[day] == NULL) {

					// Liste variables contrainte courante
					int v = 0;
					SCIP_VAR* varCons[50];

					for (auto job : s->getPosts()) {
						if (job->getId() != "Repos") {
							SCIP_VAR* var = vars.find(a->getId() + "_" + to_string(day + 1) + "_" + job->getId())->second;
							varCons[v] = var;
							v++;
						}
					}

					//Création contrainte <= 1
					SCIP_CONS* cons;
					//SCIPcreateConsBasicSetpart(scip, &cons, "1_poste_par_jour", v, varCons);
					SCIPcreateConsBasicSetpack(scip, &cons, "un_poste_par_jour", v, varCons);
					SCIPaddCons(scip, cons);

					// Ajout liste contraintes
					auto it = consVec.end();
					consVec.insert(it, cons);

					id_contrainte++;
				}
			}
		}

	}

	fileIdVar.close();

	cout << "Write init pl" << endl;
	SCIPwriteOrigProblem(scip, "init.txt", "lp", FALSE);

	SCIPsolve(scip);
	SCIP_SOL* sol = SCIPgetBestSol(scip);


	if (sol == NULL)
	{
		cout << "Solution = NULL" << endl;
	}

	//SCIPprintSol(scip, sol, NULL, TRUE);

	// Release & free +
	// Changer les calendriers des agents
	for (int id_var = 0; id_var < varVec.size(); id_var++) {
		SCIP_Real varValue = SCIPgetSolVal(scip, sol, varVec[id_var]);

			if (varValue == 1.0) {
				//cout << "we good" << " id: " << id_var << " ag: " << agentsIndex[id_var]->getId() << " j: " << daysIndex[id_var]+1 << " p: " << postsIndex[id_var]->getId() << endl;
				agentsIndex[id_var]->setCalendarDay(postsIndex[id_var], daysIndex[id_var]);
			}
			else if(varValue == .0 && agentsIndex[id_var]->getCalendar()[daysIndex[id_var]] == NULL){ //si on a pas un autre poste attribué plus tôt pour le même jour et le même agent ==> repos
				//cout << "not good " << id_var << endl;
				agentsIndex[id_var]->setCalendarDay(new Post("Repos", 0.0), daysIndex[id_var]);
			}

		SCIPreleaseVar(scip, &varVec[id_var]);
	}

	for (auto c : consVec) {
		SCIPreleaseCons(scip, &c);
	}
	SCIPfree(&scip);

	mr.printPlanning();

	return mr;

}
