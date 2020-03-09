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
	defaultPost=NULL;
}

Model::Model(const Model &obj) {
	this->firstDay=obj.firstDay;
	this->nbDays=obj.nbDays;
	this->overtime=obj.overtime;
	this->services=obj.services;
	this->posts=obj.posts;
	this->defaultPost=obj.defaultPost;

	for(auto a : obj.agents){
		for(auto b : a.second){
			agents[a.first].push_back(new Agent(*b));
		}
	}

}

Model::~Model() {
	// TODO Auto-generated destructor stub
}

const std::set<Service*>& Model::getServices() const {
	return services;
}

void Model::addAgent(Agent* agent, Service* service) {
	agents[service].push_back(agent);
	service->addAgent(agent);
}

void Model::addService(Service* service ) {
	services.insert(service);
}

void Model::printPlanning() {

	cout << "Jours:\t";
	for(int i=0;i<nbDays;i++){
		cout << "\t" << i+1;
	}
	cout << endl;
	for(auto s : agents)
	{
		for(auto agent : s.second)
		{
			cout << "Agent " << agent->getId() << ":\t";

			for(unsigned int j=0;j<agent->getCalendar().size();j++)
			{
				if(agent->getCalendar()[j]!=NULL)
				{
					cout << agent->getCalendar()[j]->getId();
				}
				else
					cout << "null";
				cout << "\t";
			}
			cout << endl;
		}


	}
}

Day Model::getFirstDay() const {
	return firstDay;
}

void Model::setFirstDay(Day firstDay) {
	this->firstDay = firstDay;
}

int Model::getNbDays() const {
	return nbDays;
}

void Model::setNbDays(int nbDays) {
	this->nbDays = nbDays;
}

float Model::getOvertime() const {
	return overtime;
}

std::vector<Agent*> Model::getAgentFrom(Service *service) {
	return agents[service];
}

void Model::setOvertime(float overtime) {
	this->overtime = overtime;
}

Day Model::getNextDay(Day day) {
	switch(day){
		case Monday:
			return Tuesday;
		case Tuesday:
			return Wednesday;
		case Wednesday:
			return Thursday;
		case Thursday:
			return Friday;
		case Friday:
			return Saturday;
		case Saturday:
			return Sunday;
		case Sunday:
			return Monday;
		default:
			return None;
	}
}

Post* Model::getDefaultPost() {
	return defaultPost;
}

void Model::setDefaultPost(Post *defaultPost) {
	this->defaultPost = defaultPost;
}
