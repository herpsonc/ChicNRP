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
	firstDay=obj.firstDay;
	nbDays=obj.nbDays;
	overtime=obj.overtime;
	services=obj.services;
	posts=obj.posts;
	defaultPost=obj.defaultPost;
	agents = map < Service*, std::vector<Agent*>>();

	for(auto a : obj.agents){
		for(auto b : a.second){
			agents[a.first].push_back(new Agent(*b));
		}
	}
}

Model::~Model() {
	for (auto a : agents)
		for (auto b : a.second) {
			delete b;
		}
	agents.clear();
}

Model& Model::operator=(const Model& obj)
{
	if (this != NULL) {
		firstDay = obj.firstDay;
		nbDays = obj.nbDays;
		overtime = obj.overtime;
		services = obj.services;
		posts = obj.posts;
		defaultPost = obj.defaultPost;
		agents = map < Service*, std::vector<Agent*>>();

		for (auto a : obj.agents) {
			for (auto b : a.second) {
				agents[a.first].push_back(new Agent(*b));
			}
		}
	}
	return Model(obj);
}


std::vector<Service*>& Model::getServices() {
	return services;
}

void Model::addAgent(Agent* agent, Service* service) {
	agents[service].push_back(agent);
	service->addAgent(agent);
}

void Model::addService(Service* service ) {
	services.push_back(service);
}

void Model::printPlanning() {

	cout << "Jours:\t";
	for(int i=0;i<nbDays;i++){
		cout << "  " << i+1;
	}
	cout << endl;
	for(auto s : agents)
	{
		for(auto agent : s.second)
		{
			cout << "Agent " << agent->getId() << ":   ";

			for(unsigned int j=0;j<agent->getCalendar().size();j++)
			{
				if(agent->getCalendar()[j]!=NULL)
				{
					cout << agent->getCalendar()[j]->getId();
				}
				else
					cout << "null";
				cout << "  ";
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

Model Model::generateModelInstance(Day firstDay, int nbDays, float overtime, int nbServices, int nbPosts, int nbAgents, float nbHoursWeek) {
	Model m = Model(firstDay, nbDays, overtime);
	m.defaultPost = NULL;
	
	int nbPostsAvailable = nbPosts, nbPostsService_i = -1, testLengthPost = -1;

	//Services et Postes

	//poste congé dispo dans chaque service
	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut("rest");

	//pour chaque service
	for (int i = 0; i < nbServices; i++){
		Service* service_i = new Service(std::to_string(i));
		m.addService(service_i);

		if (i != nbServices - 1 && nbPostsAvailable >= 2) {
			nbPostsService_i = 2 + rand() % (nbPostsAvailable / 2); //au moins 2 postes par service (nbPosts est donc approximatif)
		}
		else {
			nbPostsService_i = nbPostsAvailable; //solution temporaire : risque d'avoir bcp + de posts dans le dernier service
		}

		//pour chaque post qui sera dans ce service
		for (int j = 0; j < nbPostsService_i; j++) {
			testLengthPost = rand() % 10 + 1;
			Post* posti_j;
			if (testLengthPost >= 5) { //50% de chance de créer un poste à 12.25h, sinon 7.5h
				posti_j = new Post("P"+std::to_string(i)+"_"+std::to_string(j), 12.25);
			}
			else {
				posti_j = new Post("P" + std::to_string(i) + "_" + std::to_string(j), 7.5);
			}
			//i_j->addAttribut("workL");			//comment faire ?
			service_i->addPost(posti_j);
			//ghr->addPostRequired(posti_j, 1);     //idem
		}

		service_i->addPost(repos); //chaque service a un poste Repos

		nbPostsAvailable -= nbPostsService_i;
	}

	//ajouter contraintes ?

	
	//Agents

	//variables pour les tirages aléatoires
	int service_rand = 0, post_rand = 0, conges_rand = 0, jour_rand = 0, incr_jour = 0;

	//pour chaque agents
	for (int i = 0; i < nbAgents; i++) {
		int dice = rand() % 10+1;
		Agent* a_i;
		if (dice <= 2) { //20% de chance d'avoir un agent qui débute
			a_i = new Agent(std::to_string(i), i, nbHoursWeek, Status::Beginner);
		}
		else {
			a_i = new Agent(std::to_string(i), i, nbHoursWeek, Status::Confirmed);
		}
		service_rand = rand() % nbServices;
		a_i->setService(m.getServices()[service_rand]);

		for (int jour = 0; jour < nbDays; jour++) {
			conges_rand = rand() % 10 + 1; //30% de chance d'avoir un congé dans le mois
			cout << i << ": " << conges_rand << endl;
			if (conges_rand <= 2) {
				jour_rand = rand() % nbDays + 1; //random pour choisir le jour du congé

				a_i->setCalendarDay(repos, jour_rand, true);

				//chances de faire une suite de congés après un premier congés
				conges_rand = rand() % 10 + 1;
				cout << i << " suite de congés : " << conges_rand << endl;
				while (conges_rand <= 5 && jour_rand + incr_jour <= nbDays) { //ne pas dépasser le dernier jour du mois (pas forcément réaliste)
					a_i->setCalendarDay(repos, jour_rand + incr_jour, true);
					conges_rand = rand() % 10 + 1;
					incr_jour++;
				}
				incr_jour = 0;
			}
		}
		m.addAgent(a_i, m.getServices()[service_rand]);
	}

	return m;
}