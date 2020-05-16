/*
 * Model.cpp
 *
 *  Created on: 1 mars 2020
 *      Author: Valars
 */

#include "Model.h"

using namespace std;

Model::Model(Day firstDay, int nbDays, float overtime) {
	this->firstDay = firstDay;
	this->nbDays = nbDays;
	this->overtime = overtime;
	defaultPost = NULL;
}

Model::Model(const Model& obj) {
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
		for (auto a : agents)
			for (auto b : a.second) {
				delete b;
			}
		agents.clear();
		agents = map < Service*, std::vector<Agent*>>();

		for (auto a : obj.agents) {
			for (auto b : a.second) {
				agents[a.first].push_back(new Agent(*b));
			}
		}
	}
	return Model(obj);
}


std::vector<Service*>& Model::getServices(){
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

	auto day = firstDay;
	cout << "Jours:\t";

	for(int i=0;i<nbDays;i++){
		cout << "  " << i+1 << " ";

		switch (day) {
		case Monday:
			cout << "M";
			break;
		case Tuesday:
			cout << "T";
			break;
		case Wednesday:
			cout << "W";
			break;
		case Thursday:
			cout << "T";
			break;
		case Friday:
			cout << "F";
			break;
		case Saturday:
			cout << "S";
			break;
		case Sunday:
			cout << "S";
			break;
		default:
			cout << "";
			break;
		}

		day = getNextDay(day);
	}
	cout << endl;
	for (auto s : agents)
	{
		for (auto agent : s.second)
		{
			cout << "Agent " << agent->getId() << ":   ";

			for (unsigned int j = 0; j < agent->getCalendar().size(); j++)
			{
				if (agent->getCalendar()[j] != NULL)
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

std::vector<Agent*> Model::getAgentFrom(Service* service) {
	return agents[service];
}

void Model::setOvertime(float overtime) {
	this->overtime = overtime;
}

Day Model::getNextDay(Day day) {
	switch (day) {
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

void Model::setDefaultPost(Post* defaultPost) {
	this->defaultPost = defaultPost;
}

vector<Constraint*> Model::createConstraints() {
	std::vector<Constraint*> constraints;

	//Pas de Nuit/Jour
	auto v = vector<string>();
	v.push_back("night");
	v.push_back("day");
	constraints.push_back(new ConstraintDaysSeq(v, 1)); // cJN

	//Pas 3 jours/nuit de travail d'affilé
	v = vector<string>();
	v.push_back("workL");
	v.push_back("workL");
	v.push_back("workL");
	constraints.push_back(new ConstraintDaysSeq(v, 1)); // c3N

	//Pas de nuit avant un congé posé
	v = vector<string>();
	v.push_back("night");
	v.push_back("ca");
	constraints.push_back(new ConstraintDaysSeq(v, 1)); // crn

	//Après 2 jours/nuits au moins 2 repos
	v = vector<string>();
	v.push_back("workL");
	v.push_back("workL");
	auto v2 = vector<string>();
	v2.push_back("rest");
	v2.push_back("rest");
	constraints.push_back(new ConstraintInvolved(v, v2, 1)); // cnr

	//1 week ends par mois
	v = vector<string>();
	v.push_back("work");
	v.push_back("work");
	constraints.push_back(new ConstraintSeqMinMax(Day::Saturday, MinMax::Min, 1, v, 1));

	return constraints;
}

Model Model::generateModelInstance(Day firstDay, int nbDays, float overtime, int nbServices, int nbPosts, int nbAgents, float nbHoursWeek, float nbHoursMonth, int nbAgentsPerService, int nbPostsPerService, int proba_1er_conge, int proba_suite_conge) {
	/*
	Si nbPostsPerService est indiqué, le nombre nbPosts ne sera pas respecté si nbPosts < nbPostsPerService*nbServices
	de même pour nbAgentsPerService
	*/

	Model m = Model(firstDay, nbDays, overtime);
	m.defaultPost = NULL;

	int nbPostsAvailable = nbPosts, nbPostsService_i = -1, testLengthPost = -1, dayOrNight = -1;

	//Services et Postes

	//poste congé/repos dispos dans chaque service
	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut("rest");

	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut("rest");
	ca->addAttribut("ca");

	m.setDefaultPost(repos);

	std::vector<Constraint*> constraints = m.createConstraints(); //crée toutes les contraintes à respecter par les postes

	//si on a un nombre de postes par service précis à respecter
	if (nbPostsPerService > -1) {
		nbPostsService_i = nbPostsPerService;
	}

	//attribution de postes pour chaque service
	for (int i = 0; i < nbServices; i++) {
		Service* service_i = new Service(std::to_string(i));
		m.addService(service_i);

		service_i->addPost(repos); //ajout des postes obligatoires en dur
		//service_i->addPost(ca);

		if (nbPostsPerService == -1) {
			if (i != nbServices - 1 && nbPostsAvailable >= 2) {
				nbPostsService_i = 2 + rand() % (nbPostsAvailable / 2); //au moins 2 postes par service (nbPosts est donc approximatif)
			}
			else { //si c'est le dernier service : lui attribuer tous les posts restants
				nbPostsService_i = nbPostsAvailable; //risque d'avoir bcp + de posts dans le dernier service
			}
		}

		//pour chaque post qui sera dans ce service
		for (int j = 0; j < nbPostsService_i; j++) {
			testLengthPost = rand() % 10 + 1;
			Post* posti_j;
			if (testLengthPost >= 5) { //50% de chance de créer un poste à 12.25h, sinon 7.5h
				posti_j = new Post("S" + std::to_string(i) + "P" + std::to_string(j)+"L", 12.25);
				posti_j->addAttribut("work");
				posti_j->addAttribut("workL"); //poste à durée longue
			}
			else {
				posti_j = new Post("S" + std::to_string(i) + "P" + std::to_string(j), 7.5);
				posti_j->addAttribut("work");
			}

			dayOrNight = rand() % 3 + 1; // 2 chances /3 d'être un poste en journée
			if (dayOrNight <= 2) {
				posti_j->setId(posti_j->getId() + "D");
				posti_j->addAttribut("day");
			}
			else {
				posti_j->setId(posti_j->getId() + "N");
				posti_j->addAttribut("night");
			}
			service_i->addPost(posti_j);
			service_i->addPostRequired(posti_j, 1);
		}

		service_i->addPost(repos); //chaque service a un poste Repos
		//et on ajoute à chaque service la liste des contraintes
		for (int id_cstr = 0; id_cstr < constraints.size(); id_cstr++) {
			service_i->addConstraint(constraints[id_cstr]);
		}

		nbPostsAvailable -= nbPostsService_i;
	}

	//Agents

	//variables pour les tirages aléatoires
	int service_rand = 0, post_rand = 0, conges_rand = 0, job_rand = 0, incr_jour = 0, post_ind_rand = 0;
	if (proba_1er_conge < 0 || proba_1er_conge > 100) 
		proba_1er_conge = 3; //3% de chance d'avoir un congé si proba perso non déclarée (ou incorrecte)
	if (proba_suite_conge < 0 || proba_suite_conge > 100)
		proba_suite_conge = 70; //70% de chance d'avoir un congé à la suite d'un premier congé si proba perso non déclarée (ou incorrecte)
	

	//pour chaque agent
	for (int i = 0; i < nbAgents; i++) {
		int dice = rand() % 10 + 1;
		Agent* a_i;
		if (dice <= 2) { //20% de chance d'avoir un agent qui débute
			a_i = new Agent(std::to_string(i), nbHoursMonth, nbHoursWeek, Status::Beginner);
		}
		else {
			a_i = new Agent(std::to_string(i), nbHoursMonth, nbHoursWeek, Status::Confirmed);
		}

		//si aucun nombre précis d'agents par service n'est donné, random pour attribuer un service
		if (nbAgentsPerService == -1) {
			service_rand = rand() % nbServices;
			a_i->setService(m.getServices()[service_rand]);
		}
		// sinon, ajout de l'agent au premier service libre
		else {
			int j = 0;
			while (m.getServices()[j]->getAgents().size() >= nbAgentsPerService) {
				j++;
			}
			a_i->setService(m.getServices()[j]);
		}

		vector<Post*> posts_possibles = a_i->getService()->getPosts();

		//pour chaque jour
		for (int jour = 0; jour < nbDays; jour++) {
			conges_rand = rand() % 100 + 1;
			job_rand = rand() % 100 + 1;
			//cout << i << ": " << conges_rand << endl;
			if (conges_rand <= proba_1er_conge) { // proba_1er_conge% de chance d'avoir un premier congé ce jour ci

				a_i->setCalendarDay(ca, jour, true);

				//chances de faire une suite de congés après un premier congé = 70% puis décrément de 5% par jour à la suite
				conges_rand = rand() % 100 + 1;

				while (conges_rand <= (proba_suite_conge -(incr_jour*5)) && jour + incr_jour <= nbDays) {
					a_i->setCalendarDay(ca, jour + incr_jour, true);
					conges_rand = rand() % 100 + 1;
					//cout << conges_rand << " : " << (70 - (incr_jour * 5) ) << endl;
					incr_jour++; //permet aussi le décrément de 5% de chance par jour succéssif
				}
				jour += incr_jour; //si on a déjà ajouté un congé aux jours suivants : on passe
				incr_jour = 0;
			}
			else if(job_rand <= 5){ //5% chance d'avoir un job pré-défini ce jour là
				post_ind_rand = rand() % posts_possibles.size();
				//cout << post_ind_rand << "  " << posts_possibles.size() << endl;
				if (posts_possibles[post_ind_rand]->getId() != "Repos") {
					a_i->setCalendarDay(posts_possibles[post_ind_rand], jour, true);
				}
			}
		}

		m.addAgent(a_i, m.getServices()[service_rand]);
	}

	return m;
}