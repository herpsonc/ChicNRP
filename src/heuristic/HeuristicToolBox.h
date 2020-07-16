#pragma once

#include "../model/Model.h"

class HeuristicToolBox
{
public:
	static void checkFastDaySeq(Model* m, ConstraintDaysSeq* constraint, int service, int iCons);
	static void checkFastInvolved(Model* m, ConstraintInvolved* constraint, int service, int iCons);
	static void checkFastSeqMinMax(Model* m, ConstraintSeqMinMax* constraint, int service, int iCons);
	static void checkWorkingHoursWeekFast(Model* m, Agent* agent, int idService, int day, int idA);
	static void checkImpossiblePostsFast(Model* m, Agent* agent, int idService, int day, int idA);
	static int getNextDay(int day);

};

