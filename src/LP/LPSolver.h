
#include "../model/Model.h"
#include <cmip.h>
#include <except.h>
#include <iostream>

class LPSolver {
public:
	LPSolver();
	virtual ~LPSolver();

	static Model linearProgramTest(const Model m);
	static void nullTo(Model* m, Post* post);
};

