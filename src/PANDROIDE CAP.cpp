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
#include "model/constraint/ConstraintFactory.h"
using namespace std;


int main() {
	Agent *a = new Agent();
	a->setNbHours(155);
	cout << "nbHeure de l'agent: " << a->getNbHours() << endl;

	vector<Post> vp = vector<Post>();
	Constraint* c = ConstraintFactory::createConstraintDaysSeq(vp, 15);

	cout << "priorite:   " << c->getPriority() << endl;;
	return 0;
}
