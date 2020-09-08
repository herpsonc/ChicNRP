
#include <iostream>
#include <vector>
#include <algorithm>

#include "heuristic/heuristicSolver.h"
#include "LP/LPSolver.h"
#include "HandWrittedModels.h"

using namespace std;

int main() {

	//Important pour initialiser l'aléatoire
	srand(time(0));

	//auto m = HandWrittedModels::generateApril20();
	//m.printPlanning();

	auto m3 = heuristicSolver::iterativeFast(HandWrittedModels::generateApril20(), 10000, 4, 3);
	m3.printPlanning();
	m3.generateXlsx("AllService2.xml");
	m3.generateEmptyValuation();
	HeuristicToolBox::checkAllFast(&m3);
	cout << "score " << m3.getValuation()->getScore() << endl;
	cout << m3.getConstraintInformations() << endl;

	/*Model m2 = Model::generateModelInstance(2, 30, 25, 6, 15, 70, 60.0, 155);
	m2.printPlanning();
	m2.generateXlsx("service_g6_PL.xlsx");
	m2 = LPSolver::linearProgram(m2);*/

	string t;
	cin >> t;

	return 0;
}