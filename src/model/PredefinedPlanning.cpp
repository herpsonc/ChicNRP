#include "PredefinedPlanning.h"

PredefinedPlanning::PredefinedPlanning()
{
	
}

void PredefinedPlanning::addAgent()
{
	planning.push_back(array<Post*, 31>({ NULL }));
}

void PredefinedPlanning::setPlanningDay(Post* post,const int idAgent, const int day)
{
	planning[idAgent][day] = post;
}

const std::vector<std::array<Post*, 31>>* PredefinedPlanning::getPlanning()
{
	return &planning;
}
