#pragma once

#include "../model/Model.h"

class HeuristicToolBox
{
public:
	static void checkFastDaySeq(Model* m, ConstraintDaysSeq* constraint, int iCons);
	static void checkFastInvolved(Model* m, ConstraintInvolved* constraint, int iCons);
	static void checkFastSeqMinMax(Model* m, ConstraintSeqMinMax* constraint, int iCons);

};

