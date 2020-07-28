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
	static void checkPostsRequirementFast(Model* m, int idService, int day);
	static void checkAllFast(Model* m);
	static int getNextDay(int day);

	static void checkAllDaySeq(Model* m);
	static void checkAllInvolved(Model* m);
	static void checkAllSeqMinMax(Model* m);
};

