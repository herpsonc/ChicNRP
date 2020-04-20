#include "LPSolver.h"

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

	const int n = n_tmp, m = m_tmp, nz = n_tmp*m_tmp;

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

		int id_contrainte = 0;
		int id_var = 0; //agent
		for (int day = 0; day < mr.getNbDays(); day++) {

			for (auto s : mr.getServices()) {

				//défaut : prend pas en compte les agents qu'on peut solliciter en dehors du service où ce poste est dédié en général
				vector<Agent*> agents = mr.getAgentFrom(s); 

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
								cout << "X_a" << a->getId() << ",j" << day+1 << "," << job->getId() << "= x(" << id_var << ")" << endl;

								//prob.addVar(id_var, CMIP::VAR_INT, 1, 0.0, 1.0);  // 0 <= 1*x_i <= 1   : var binaire (fonction objectif)
								SCIP_VAR* var;

								SCIPcreateVar(scip, &var, NULL, 0.0, 1.0, 1.0, SCIP_VARTYPE_BINARY, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL);
								SCIPaddVar(scip, var);
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
							SCIPcreateConsSetpart(scip, &cons, "agent_dispo", v, varCons, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE);
							auto it = consVec.end();
							consVec.insert(it, cons);

							SCIPprintCons(scip, cons, NULL);
							//cout << endl;
						}

						id_contrainte++;
					}
				}
				//un agent ne peut faire qu'un poste par jour
				//for (auto a : agents) {
				//	if (a->getCalendar()[day] == NULL) {

				//		prob.addCtr(id_contrainte, 0, 1, 1); //contrainte_jour_j_poste_p = 1

				//		for (auto job : s->getPosts()) {
				//			if (job->getId() != "Repos") {

				//			}
				//		}
				//		id_contrainte++;
				//	}
				//}
			}

		}

		cout << id_contrainte << endl;
		/*
	}

	catch (CException * pe) {
		std::cerr << pe->what() << std::endl;
		delete pe;
		return mr;
	}*/

		FILE* file;
		file = fopen("LPTest.txt", "w+");
		for (auto v : varVec) {
			SCIPprintVar(scip, v, NULL);
		}

		for (auto c : consVec) {
			SCIPprintCons(scip, c, NULL);
			cout << endl;
		}

		SCIPsolve(scip);
		SCIP_SOL* sol = SCIPgetBestSol(scip);


		if (sol == NULL)
		{
			cout << "Solution = NULL" << endl;
		}

		//SCIPgetProbData(scip);
		//SCIPprintOrigProblem(scip, file, NULL, TRUE);
		//SCIPprintLPStatistics(scip, file);

		SCIPprintSol(scip, sol, NULL, TRUE);



	return mr;

}


void nullTo(Model* m, Post* post) {

}