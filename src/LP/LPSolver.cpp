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

	try {
		CMIP prob("PL_GHR_test"); //nouveau problème
		prob.openMatrix(n, m, nz); //matrice de dim m*n : m = nb_contraintes, n = nb_vars, nz = nb_valeurs des coefficients  à 0 pour chaque var dans chaque contrainte
		prob.setObjSense(false); //minimiser au lieu de maximiser

		int id_contrainte = 0;
		int id_var = 0; //agent
		for (int day = 0; day < mr.getNbDays(); day++) {

			for (auto s : mr.getServices()) {

				//défaut : prend pas en compte les agents qu'on peut solliciter en dehors du service où ce poste est dédié en général
				vector<Agent*> agents = mr.getAgentFrom(s); 

				for (auto job : s->getPosts()) {
					if (job->getId() != "Repos") {

						bool agent_dispo = false;
						for (auto a : agents) {
							if (a->getCalendar()[day] == NULL) {
								if (!agent_dispo) { //permet d'ajouter une contrainte seulement s'il y a au moins un agent dispo pour ce job à cette date
									prob.addCtr(id_contrainte, 0, 1, 1); //contrainte_jour_j_poste_p = 1
									agent_dispo = true;
								}
								cout << "X_a" << a->getId() << ",j" << day+1 << "," << job->getId() << "= x(" << id_var << ")" << endl;

								prob.addVar(id_var, CMIP::VAR_INT, 1, 0.0, 1.0);  // 0 <= 1*x_i <= 1   : var binaire (fonction objectif)

								prob.addEntry(1, id_contrainte, id_var); //x_day,job,a0 + x_day,job,a1 + ...

								id_var++;
							}
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

		prob.printMatrix("LPTestMatrix.txt", true);
		prob.closeMatrix(); // fermeture de la matrice
		prob.optimize(); //  résolution du PL
		prob.printSolution("LPTest2.sol"); //print
	}
	catch (CException * pe) {
		std::cerr << pe->what() << std::endl;
		delete pe;
		return mr;
	}

	return mr;

	/*
	try {
		CMIP prob("MIPCLtest"); //nouveau problème
		prob.openMatrix(n, m, nz); //matrice de dim n, m, avec nz valeurs != 0
		
		//Ajout de variables à la fonction objectif. Paramètres : numéro, type, coefficient, borne inf, borne sup
		prob.addVar(0, CMIP::VAR_INT, 100.0, 0.0, CLP::VAR_INF); //100*x0
		prob.addVar(1, CMIP::VAR_INT, 64.0, 0.0, CLP::VAR_INF);  //+64*x1

		//contraintes
		prob.addCtr(0, 0, -CLP::INF, 250); //contrainte0 < 250
		prob.addCtr(1, 0, -CLP::INF, 4); // contrainte1 < 4

		//valeur des coefficients
		prob.addEntry(50.0, 0, 0); //contrainte0 : 50x0
		prob.addEntry(31.0, 0, 1); //			  +31x1
		prob.addEntry(-3.0, 1, 0); //contrainte1 : -3x0
		prob.addEntry(2.0, 1, 1); //			   +2x1

		prob.closeMatrix(); //
		prob.optimize(); //  résolution du PL
		prob.printSolution("primer.sol"); //print
	}
	catch (CException * pe) {
		std::cerr << pe->what() << std::endl;
		delete pe;
		return m;
	}

	return m;*/
}


void nullTo(Model* m, Post* post) {

}