/*
 * Model.cpp
 *
 *  Created on: 1 mars 2020
 *      Author: Valars
 */

#include "Model.h"
#include "../rapidXml/rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

Model::Model(Day firstDay, int nbDays, float overtime) {
	this->firstDay=firstDay;
	this->nbDays=nbDays;
	this->overtime=overtime;
	swapLog = vector<SwapLog>();
	defaultPost=NULL;
	valuation = new Valuation();
}

Model::Model(const Model &obj) {
	firstDay=obj.firstDay;
	nbDays=obj.nbDays;
	overtime=obj.overtime;
	services=obj.services;
	posts=obj.posts;
	defaultPost=obj.defaultPost;
	swapLog = obj.swapLog;
	valuation = new Valuation(*obj.valuation);
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
	
	delete valuation;

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
		swapLog = obj.swapLog;
		if (valuation) {
			delete valuation;
		}
		valuation = new Valuation(*obj.valuation);
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

//! \return services vector of Services of the model
std::vector<Service*>& Model::getServices(){
	return services;
}

//! Add an agent to the service
//! \param agent Agent to add
//! \param service Service where the agent is added
void Model::addAgent(Agent* agent, Service* service) {
	agents[service].push_back(agent);
	service->addAgent(agent);
}

//! Add a service to the model
//! \param service Service to add
void Model::addService(Service* service ) {
	services.push_back(service);
}

//! Print the planning to the console
void Model::printPlanning() {

	auto day = firstDay;
	cout << "Jours:\t";

	for(int i=0;i<nbDays;i++){
		cout << "\t" << i+1 << " ";

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
		cout << "---------------------------" << endl;
		cout << "Service " << s.first->getId() << endl;
		cout << "---------------------------" << endl;
		for (auto agent : s.second)
		{
			cout << "Agent " << agent->getId() << ":\t";

			for (unsigned int j = 0; j < agent->getCalendar().size(); j++)
			{
				if (agent->getCalendar()[j] != NULL)
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

//! \return firstDay first day of the month
Day Model::getFirstDay() const {
	return firstDay;
}

//! \param firstday first day to set
void Model::setFirstDay(Day firstDay) {
	this->firstDay = firstDay;
}

//! \return nbDays number of days in the month
int Model::getNbDays() const {
	return nbDays;
}

//! \param nbDays number of days to set
void Model::setNbDays(int nbDays) {
	this->nbDays = nbDays;
}

//! \return overtime overtime allowed for every Agents 
float Model::getOvertime() const {
	return overtime;
}

//! \param overtime overtime to set
void Model::setOvertime(float overtime) {
	this->overtime = overtime;
}

//! get Agents from a specific service
//! \param service the Agents returned are from this Service 
//! \return agents from the specified service
std::vector<Agent*> Model::getAgentFrom(Service* service) {
	return agents[service];
}


//! Give the next day of the given day
//! \param day 
//! \return Next next day of the given day
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

//! \return defaultPost post affected by default
Post* Model::getDefaultPost() {
	return defaultPost;
}

//! \param defaultPost Post to set
void Model::setDefaultPost(Post* defaultPost) {
	this->defaultPost = defaultPost;
}

//! \return valuation Valuation of the current instance
Valuation* Model::getValuation(){
	return valuation;
}

//! \param v valuation to set
void Model::setValuation(Valuation v){
	delete valuation;
	valuation = new Valuation(v);
}

//! Add a SwapLog to the list
//! \param swapLog SwapLog to add to the list
void Model::addSwapLog(const SwapLog swapLog){
	this->swapLog.push_back(swapLog);
}

//! \return swaplog vector of swaplog
vector<SwapLog> Model::getSwapLog(){
	return swapLog;
}

//! clear the swapLog vector
void Model::resetSwapLog(){
	swapLog.clear();
}

vector<Constraint*> Model::createConstraints() {
	std::vector<Constraint*> constraints;

	//Pas de Nuit/Jour
	auto v = vector<string>();
	v.push_back("night");
	v.push_back("day");
	constraints.push_back(new ConstraintDaysSeq(v, 30)); // cJN

	//Pas 3 jours/nuit de travail d'affilé
	v = vector<string>();
	v.push_back("workL");
	v.push_back("workL");
	v.push_back("workL");
	constraints.push_back(new ConstraintDaysSeq(v, 50)); // c3N

	//Pas de nuit avant un congé posé
	v = vector<string>();
	v.push_back("night");
	v.push_back("ca");
	constraints.push_back(new ConstraintDaysSeq(v, 30)); // crn

	//Après 2 jours/nuits au moins 2 repos
	v = vector<string>();
	v.push_back("workL");
	v.push_back("workL");
	auto v2 = vector<string>();
	v2.push_back("rest");
	v2.push_back("rest");
	constraints.push_back(new ConstraintInvolved(v, v2, Day::None, 30)); // cnr

	//Après 1 journée longue + 1 repos  -> +1 repos min
	v = vector<string>();
	v.push_back("workL");
	v.push_back("rest");
	v2 = vector<string>();
	v2.push_back("rest");
	constraints.push_back(new ConstraintInvolved(v, v2, Day::None, 20)); //cwl2r

	//Evite les journée isolés
	v = vector<string>();
	v.push_back("rest");
	v.push_back("work");
	v.push_back("rest");
	constraints.push_back(new ConstraintDaysSeq(v, 10)); //cji

	//1 week ends par mois
	v = vector<string>();
	v.push_back("work");
	v.push_back("work");
	constraints.push_back(new ConstraintSeqMinMax(Day::Saturday, MinMax::Min, 1, v, 1));

	return constraints;
}

//raccourci pour attribuer les contraintes à un service via createConstraints
void Model::addBasicConstraintsTo(Service* s) {
	vector<Constraint*> cs = this->createConstraints();
	for each (auto cons in cs){
		s->addConstraint(cons);
	}
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
				nbPostsService_i = rand() % (nbPostsAvailable / nbServices); //nbPosts entre 0 (2 en réalité) et nb total des posts/nbServices pour essayer d'avoir quelque chose d'équilibré
			}
			else { //si c'est le dernier service : lui attribuer tous les posts restants
				nbPostsService_i = nbPostsAvailable; //risque d'avoir bcp + de posts dans le dernier service
			}
		}

		cout << nbPostsService_i << endl;

		if (nbPostsService_i < 2) {
			nbPostsService_i = 2; //empêche d'avoir des services sans postes mais => non respect de la valeur nbPosts des fois
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

//! Generate a XML file from the Model
//! \param fileName name of the xml file saved
void Model::generateXML(string fileName){

	xml_document<> doc;

	xml_node<>* root = doc.allocate_node(node_element, "Model");

	root->append_attribute(doc.allocate_attribute("firstDay", doc.allocate_string(to_string(firstDay).c_str())));
	root->append_attribute(doc.allocate_attribute("nbDays", doc.allocate_string(to_string(nbDays).c_str())));
	root->append_attribute(doc.allocate_attribute("overtime", doc.allocate_string(to_string(overtime).c_str())));

	//DefaultPost
	xml_node<>* defaultP = doc.allocate_node(node_element, "DefaultPost");
	defaultP->append_attribute(doc.allocate_attribute("id", doc.allocate_string(defaultPost->getId().c_str())));
	defaultP->append_attribute(doc.allocate_attribute("nbh", doc.allocate_string(to_string(defaultPost->getTime()).c_str())));


	//Attributs du post
	for (auto attri : defaultPost->getAttributs()) {
		xml_node<>* att = doc.allocate_node(node_element, "Attribut");
		att->append_attribute(doc.allocate_attribute("Attribut", doc.allocate_string(attri.c_str())));
		defaultP->append_node(att);
	}

	root->append_node(defaultP);

	//Chaque Services
	for (auto s : agents) {
		xml_node<>* service = doc.allocate_node(node_element, "Service");
		service->append_attribute(doc.allocate_attribute("id", doc.allocate_string(s.first->getId().c_str())));

		//On met les posts
		for (auto p : s.first->getPosts()) {
			xml_node<>* post = doc.allocate_node(node_element, "Post");
			post->append_attribute(doc.allocate_attribute("id", doc.allocate_string(p->getId().c_str())));
			post->append_attribute(doc.allocate_attribute("time", doc.allocate_string(to_string(p->getTime()).c_str())));

			//Attributs du post
			for (auto a : p->getAttributs()) {
				xml_node<>* att = doc.allocate_node(node_element, "Attribut");
				att->append_attribute(doc.allocate_attribute("Attribut", doc.allocate_string(a.c_str())));
				post->append_node(att);
			}
			service->append_node(post);
		}

		//TODO REFERENTS

		//PostRequired
		for (auto day : s.first->getPostRequired()) {
			xml_node<>* requiredPost = doc.allocate_node(node_element, "RequiredPost");
			for (auto p : day) {
				xml_node<>* post = doc.allocate_node(node_element, "Post");
				post->append_attribute(doc.allocate_attribute("id", doc.allocate_string(p.first->getId().c_str())));
				post->append_attribute(doc.allocate_attribute("nbh", doc.allocate_string(to_string(p.second).c_str())));
				requiredPost->append_node(post);
			}
			service->append_node(requiredPost);
		}

		//Constraintes du service
		for (auto c : s.first->getConstraints()) {
			if (typeid(*c) == typeid(ConstraintDaysSeq)) {
				xml_node<>* constraint = doc.allocate_node(node_element, "ConstraintDaySeq");
				constraint->append_attribute(doc.allocate_attribute("priority", doc.allocate_string(to_string(((ConstraintDaysSeq*)c)->getPriority()).c_str())));
				for (auto sA : ((ConstraintDaysSeq*)c)->getSequenceAtt()) {
					xml_node<>* att = doc.allocate_node(node_element, "Attribut");
					att->append_attribute(doc.allocate_attribute("Att", doc.allocate_string(sA.c_str())));
					constraint->append_node(att);
				}
				service->append_node(constraint);
			}
			else if (typeid(*c) == typeid(ConstraintInvolved)) {
				xml_node<>* constraint = doc.allocate_node(node_element, "ConstraintInvolved");
				constraint->append_attribute(doc.allocate_attribute("priority", doc.allocate_string(to_string(((ConstraintInvolved*)c)->getPriority()).c_str())));
				constraint->append_attribute(doc.allocate_attribute("day", doc.allocate_string(to_string(((ConstraintInvolved*)c)->getFirstDay()).c_str())));
				for (auto fs : ((ConstraintInvolved*)c)->getFirstSeqAtt()) {
					xml_node<>* att = doc.allocate_node(node_element, "FirstAttribut");
					att->append_attribute(doc.allocate_attribute("Att", doc.allocate_string(fs.c_str())));
					constraint->append_node(att);
				}
				for (auto fs : ((ConstraintInvolved*)c)->getLastSeqAtt()) {
					xml_node<>* att = doc.allocate_node(node_element, "LastAttribut");
					att->append_attribute(doc.allocate_attribute("Att", doc.allocate_string(fs.c_str())));
					constraint->append_node(att);
				}
				service->append_node(constraint);
				
			}
			else if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
				xml_node<>* constraint = doc.allocate_node(node_element, "ConstraintSeqMinMax");
				constraint->append_attribute(doc.allocate_attribute("priority", doc.allocate_string(to_string(((ConstraintSeqMinMax*)c)->getPriority()).c_str())));
				constraint->append_attribute(doc.allocate_attribute("day", doc.allocate_string(to_string(((ConstraintSeqMinMax*)c)->getFirstDay()).c_str())));
				constraint->append_attribute(doc.allocate_attribute("number", doc.allocate_string(to_string(((ConstraintSeqMinMax*)c)->getNumber()).c_str())));
				constraint->append_attribute(doc.allocate_attribute("type", doc.allocate_string(to_string(((ConstraintSeqMinMax*)c)->getType()).c_str())));
				for (auto fs : ((ConstraintSeqMinMax*)c)->getSequenceAtt()) {
					xml_node<>* att = doc.allocate_node(node_element, "Attribut");
					att->append_attribute(doc.allocate_attribute("Att", doc.allocate_string(fs.c_str())));
					constraint->append_node(att);
				}

				service->append_node(constraint);
			}
		}

		//Agents
		for (auto a : s.second) {
			xml_node<>* agent = doc.allocate_node(node_element, "Agent");
			agent->append_attribute(doc.allocate_attribute("id", doc.allocate_string(a->getId().c_str())));
			agent->append_attribute(doc.allocate_attribute("status", doc.allocate_string(to_string(a->getStatus()).c_str())));
			agent->append_attribute(doc.allocate_attribute("nbHoursMonth", doc.allocate_string(to_string(a->getNbHoursMonth()).c_str())));
			agent->append_attribute(doc.allocate_attribute("nbHoursWeek", doc.allocate_string(to_string(a->getNbHoursWeek()).c_str())));

			//ImpossiblePosts
			for (auto ip : a->getImpossiblePosts()) {
				xml_node<>* post = doc.allocate_node(node_element, "ImpossiblePost");
				post->append_attribute(doc.allocate_attribute("id", doc.allocate_string(ip->getId().c_str())));
				post->append_attribute(doc.allocate_attribute("nbh", doc.allocate_string(to_string(ip->getTime()).c_str())));

				//Attributs du post
				for (auto attri : ip->getAttributs()) {
					xml_node<>* att = doc.allocate_node(node_element, "Attribut");
					att->append_attribute(doc.allocate_attribute("Attribut", doc.allocate_string(attri.c_str())));
					post->append_node(att);
				}
				agent->append_node(post);
			}


			//On remplit le calendrier
			for (int i = 0; i < 31; i++) {
				xml_node<>* day = doc.allocate_node(node_element, "Day");
				Post* p = a->getCalendar()[i];
				if (p != NULL) {
					day->append_attribute(doc.allocate_attribute("id", doc.allocate_string(p->getId().c_str())));
					day->append_attribute(doc.allocate_attribute("nbh", doc.allocate_string(to_string(p->getTime()).c_str())));

					//Attributs du post
					for (auto attri : p->getAttributs()) {
						xml_node<>* att = doc.allocate_node(node_element, "Attribut");
						att->append_attribute(doc.allocate_attribute("Attribut", doc.allocate_string(attri.c_str())));
						day->append_node(att);
					}
				}
				else {
					day->append_attribute(doc.allocate_attribute("id", "null"));
				}
				day->append_attribute(doc.allocate_attribute("lock", doc.allocate_string(to_string(a->getCalendarLock()[i]).c_str())));
				agent->append_node(day);
			}
			service->append_node(agent);
		}



		root->append_node(service);

	}
	doc.append_node(root);

	string xml_as_string;
	rapidxml::print(back_inserter(xml_as_string), doc);
	ofstream fileStored (fileName);
	fileStored << xml_as_string;
	fileStored.close();
	doc.clear();

}

//! Load the given XML file to the Model
//! \param fileName name of the file to load
void Model::loadXML(string fileName){

	//Reset du model
	for (auto a : agents)
		for (auto b : a.second) {
			delete b;
		}
	agents.clear();
	services.clear();
	posts.clear();

	xml_document<> doc;
	rapidxml::file<> xmlFile(fileName.c_str());
	doc.parse<0>(xmlFile.data());

	//Pour garder la trace des nouveaux posts crées
	 auto mapPosts = map<string, Post*>();

	//Model
	xml_node<>* root = doc.first_node();

	firstDay = (Day)atoi(root->first_attribute("firstDay")->value());
	nbDays = atoi(root->first_attribute("nbDays")->value());
	overtime = atof(root->first_attribute("overtime")->value());

	//DefaultPost
	xml_node<>* defaultPost = root->first_node("DefaultPost");
	Post* post = new Post(defaultPost->first_attribute("id")->value(), atof(defaultPost->first_attribute("nbh")->value()));
	//attributs
	for (auto attNode = defaultPost->first_node("Attribut"); attNode; attNode = attNode->next_sibling("Attribut")) {
		post->addAttribut(attNode->first_attribute("Attribut")->value());
	}

	this->defaultPost = post;

	mapPosts.insert(pair<string, Post*>(post->getId(), post));

	//Services
	for (auto serviceNode = root->first_node("Service"); serviceNode; serviceNode = serviceNode->next_sibling("Service")) {
		auto service = new Service(serviceNode->first_attribute("id")->value());
		addService(service);

		//Posts
		for (auto postNode = serviceNode->first_node("Post"); postNode; postNode = postNode->next_sibling("Post")) {
			//On check si il existe déjà
			auto search = mapPosts.find(postNode->first_attribute("id")->value());

			//Si il n'y est pas, on la crée
			if (search == mapPosts.end()) {
				Post* post = new Post(postNode->first_attribute("id")->value(), atof(postNode->first_attribute("time")->value()));
				//attributs
				for (auto attNode = postNode->first_node("Attribut"); attNode; attNode = attNode->next_sibling("Attribut")) {
					post->addAttribut(attNode->first_attribute("Attribut")->value());
				}
				service->addPost(post);
				mapPosts.insert(pair<string, Post*>(post->getId(), post));
			}
			//Si il y est
			else {
				service->addPost(mapPosts[postNode->first_attribute("id")->value()]);
			}
		}


		//PostsRequired
		int i = 0;
		for (auto postRequiredNode = serviceNode->first_node("RequiredPost"); postRequiredNode; postRequiredNode = postRequiredNode->next_sibling("RequiredPost")) {
			for (auto post = postRequiredNode->first_node("Post"); post; post = post->next_sibling("Post")) {
				//Les posts sont normalement dans la liste de post
				service->addPostRequired(mapPosts[post->first_attribute("id")->value()], atoi(post->first_attribute("nbh")->value()),i);
			}
			i++;
		}

		//Contraintes du service
		for (auto constraintDaySeqNode = serviceNode->first_node("ConstraintDaySeq"); constraintDaySeqNode; constraintDaySeqNode = constraintDaySeqNode->next_sibling("ConstraintDaySeq")) {
			auto v = vector<string>();
			for (auto att = constraintDaySeqNode->first_node("Attribut"); att; att = att->next_sibling("Attribut")) {
				v.push_back(att->first_attribute("Att")->value());
			}
			service->addConstraint(new ConstraintDaysSeq(v, atoi(constraintDaySeqNode->first_attribute("priority")->value())));
		}

		for (auto constraintInvolved = serviceNode->first_node("ConstraintInvolved"); constraintInvolved; constraintInvolved = constraintInvolved->next_sibling("ConstraintInvolved")) {
			auto v = vector<string>();
			auto v2 = vector<string>();
			for (auto att = constraintInvolved->first_node("FirstAttribut"); att; att = att->next_sibling("FirstAttribut")) {
				v.push_back(att->first_attribute("Att")->value());
			}
			for (auto att = constraintInvolved->first_node("LastAttribut"); att; att = att->next_sibling("LastAttribut")) {
				v2.push_back(att->first_attribute("Att")->value());
			}
			service->addConstraint(new ConstraintInvolved(v, v2, (Day)atoi(constraintInvolved->first_attribute("day")->value()), atoi(constraintInvolved->first_attribute("priority")->value())));
		}

		for (auto constraintSeqMinMax = serviceNode->first_node("ConstraintSeqMinMax"); constraintSeqMinMax; constraintSeqMinMax = constraintSeqMinMax->next_sibling("ConstraintSeqMinMax")) {
			auto v = vector<string>();
			for (auto att = constraintSeqMinMax->first_node("Attribut"); att; att = att->next_sibling("Attribut")) {
				v.push_back(att->first_attribute("Att")->value());
			}
			service->addConstraint(new ConstraintSeqMinMax((Day)atoi(constraintSeqMinMax->first_attribute("day")->value()), (MinMax)atoi(constraintSeqMinMax->first_attribute("type")->value()),
				atoi(constraintSeqMinMax->first_attribute("number")->value()), v, atoi(constraintSeqMinMax->first_attribute("priority")->value())));
		}

		//Agents
		for (auto agentNode = serviceNode->first_node("Agent"); agentNode; agentNode = agentNode->next_sibling("Agent")) {
			auto agent = new Agent(agentNode->first_attribute("id")->value(), atoi(agentNode->first_attribute("nbHoursMonth")->value()),
				atoi(agentNode->first_attribute("nbHoursWeek")->value()), (Status)atoi(agentNode->first_attribute("status")->value()));

			//Calendrier
			int i = 0;
			for (auto dayNode = agentNode->first_node("Day"); dayNode; dayNode = dayNode->next_sibling("Day")) {
				if ((string)dayNode->first_attribute("id")->value() != "null") {
					auto search = mapPosts.find(dayNode->first_attribute("id")->value());

					//Si il n'y est pas, on la crée
					if (search == mapPosts.end()) {
						Post* post = new Post(dayNode->first_attribute("id")->value(), atof(dayNode->first_attribute("nbh")->value()));
						//attributs
						for (auto attNode = dayNode->first_node("Attribut"); attNode; attNode = attNode->next_sibling("Attribut")) {
							post->addAttribut(attNode->first_attribute("Attribut")->value());
						}
						mapPosts.insert(pair<string, Post*>(post->getId(), post));
					}

					agent->setCalendarDay(mapPosts[dayNode->first_attribute("id")->value()], i, dayNode->first_attribute("lock")->value());
				}
				i++;
			}

			//ImpossiblePosts
			auto v = vector<Post*>();
			for (auto impossiblePost = agentNode->first_node("ImpossiblePost"); impossiblePost; impossiblePost = impossiblePost->next_sibling("ImpossiblePost")) {
				//On check si il existe déjà
				auto search = mapPosts.find(impossiblePost->first_attribute("id")->value());

				//Si il n'y est pas, on la crée
				if (search == mapPosts.end()) {
					Post* post = new Post(impossiblePost->first_attribute("id")->value(), atof(impossiblePost->first_attribute("nbh")->value()));
					//attributs
					for (auto attNode = impossiblePost->first_node("Attribut"); attNode; attNode = attNode->next_sibling("Attribut")) {
						post->addAttribut(attNode->first_attribute("Attribut")->value());
					}
					mapPosts.insert(pair<string, Post*>(post->getId(), post));
				}
				v.push_back(mapPosts[impossiblePost->first_attribute("id")->value()]);
			}
			agent->setImpossiblePosts(v);

			addAgent(agent, service);
		}
	}
}
