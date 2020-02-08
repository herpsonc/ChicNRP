//============================================================================
// Name        : PANDROIDE.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <iostream>
#include "model/Agent.h"
using namespace std;


int main() {
	Agent *a = new Agent();
	a->setNbHours(155);
	cout << "nbHeure de l'agent: " << a->getNbHours() << endl;
	return 0;
}
