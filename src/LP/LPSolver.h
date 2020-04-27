
#include "../model/Model.h"
#include <iostream>
#include <fstream>

#include <scip/scip.h>
#include <scip/scipdefplugins.h>

class LPSolver {
public:
	LPSolver();
	virtual ~LPSolver();

	static Model linearProgramTest(const Model m);
	static void nullTo(Model* m, Post* post);
};

