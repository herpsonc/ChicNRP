#include "PredefinedPlanning.h"


using namespace std;
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

//! print planning in the output stream
void PredefinedPlanning::printPlanning()
{
	int i = 0;
	for (auto agent : planning)
	{
		cout << "Agent " << i << ":\t";

		for (unsigned int j = 0; j < agent.size(); j++)
		{
			if (agent[j] != NULL)
			{
				cout << agent[j]->getId();
			}
			else
				cout << "null";
			cout << "\t";
		}
		cout << endl;
		i++;
	}
}
