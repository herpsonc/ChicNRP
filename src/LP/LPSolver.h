
#include "../model/Model.h"
#include "VariableData.h"
#include <iostream>
#include <fstream>

#include <scip/scip.h>
#include <scip/scipdefplugins.h>

class LPSolver {
public:
	LPSolver();
	virtual ~LPSolver();

	static Model linearProgram(const Model m);
};

