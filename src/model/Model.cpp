/*
 * Model.cpp
 *
 *  Created on: 1 mars 2020
 *      Author: Valars
 */

#include "Model.h"

using namespace std;

Model::Model(Day firstDay, int nbDays, float overtime) {
	this->firstDay=firstDay;
	this->nbDays=nbDays;
	this->overtime=overtime;
}

Model::~Model() {
	// TODO Auto-generated destructor stub
}

const std::vector<Agent*>& Model::getAgents() const {
	return agents;
}

const std::vector<Service*>& Model::getServices() const {
	return services;
}

void Model::addAgent(Agent* agent) {
	agents.push_back(agent);
}

void Model::addService(Service* service ) {
	services.push_back(service);
}

void Model::printPlanning() {
	for(unsigned int i=0;i<agents.size();i++)
	{
		Agent* agent = agents[i];
		cout << "Agent " << agent->getId() << ":\t";

		for(unsigned int j=0;j<agent->getCalendar().size();j++)
		{
			if(agent->getCalendar()[j]!=NULL)
			{
				cout << j << " " << agent->getCalendar()[j]->getId();
			}
			else
				cout << j <<" null";
			cout << "\t";
		}
		cout << endl;

	}
}
