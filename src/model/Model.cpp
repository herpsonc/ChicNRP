#include "Model.h"

using namespace std;
using namespace rapidxml;

Model::Model(int firstDay, int nbDays, float overtime) {
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
	if(this) {
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
	return *this;
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
		case 0:
			cout << "M";
			break;
		case 1:
			cout << "T";
			break;
		case 2:
			cout << "W";
			break;
		case 3:
			cout << "Th";
			break;
		case 4:
			cout << "F";
			break;
		case 5:
			cout << "Sat";
			break;
		case 6:
			cout << "Su";
			break;
		default:
			cout << "";
			break;
		}

		if (day >= 0)
		day = (day + 1) % 7;
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

			for (int j = 0; j < nbDays; j++)
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
int Model::getFirstDay() const {
	return firstDay;
}

//! \param firstday first day to set
void Model::setFirstDay(int firstDay) {
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

//! return the day (Monday, Thursday...) of the idDay (0 to nbDays)
int Model::idDayToDay(int idDay)
{
	return (idDay -1 + firstDay) % 7;
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
const std::vector<Agent*> Model::getAgentFrom(Service* service){
	return agents[service];
}

//! get Agents from a specific service, give a pointer to avoid copy
//! \param service the Agents returned are from this Service 
//! \return agents from the specified service
const std::vector<Agent*>* Model::getAgentFromPtr(Service* service)
{
	return &agents[service];
}

//! \return defaultPost post affected by default
Post* Model::getDefaultPost() {
	return defaultPost;
}

//! \param defaultPost Post to set
void Model::setDefaultPost(Post* defaultPost) {
	this->defaultPost = defaultPost;
}

//! \return posts posts of the model
std::vector<Post*>& Model::getPosts() {
	return posts;
}

//! Add a post to the model, avoid duplication
void Model::addPost(Post* p) {
	if(std::find(posts.begin(), posts.end(), p) == posts.end()) //p not already in posts
		posts.push_back(p);
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

//! Generate / initialize an empty valuation for the model
void Model::generateEmptyValuation()
{
	float score = 0;
	//Pour Valuation
	int iS = 0;
	int iA = 0;
	auto hoursMonth = vector<vector<float>>();
	auto hoursWeeks = vector<vector<array<int, 6>>>();
	auto hoursWeeksSlide = vector<vector<vector<pair<int, int>>>>();
	auto impossiblePosts = vector<vector<vector<int>>>();
	auto daySeq = vector < vector<vector<vector<pair<int, int>>>>>();
	auto involved = vector<vector<vector<vector<pair<pair<int, int>, pair<int, int>>>>>>();
	auto seqMinMax = vector<vector<vector<vector<pair<int, int>>>>>();
	auto postRequirements = vector<array<int, 31>>();

	for (auto s : getServices()) {
		iA = 0;
		hoursMonth.push_back(vector<float>());
		hoursWeeks.push_back(vector<array<int, 6>>());
		hoursWeeksSlide.push_back(vector<vector<pair<int, int>>>());
		impossiblePosts.push_back(vector<vector<int>>());
		daySeq.push_back(vector<vector<vector<pair<int, int>>>>());
		involved.push_back(vector<vector<vector<pair<pair<int, int>, pair<int, int>>>>>());
		seqMinMax.push_back(vector<vector<vector<pair<int, int>>>>());
		postRequirements.push_back(array<int, 31>({ 0 }));

		for (auto a : getAgentFrom(s)) {

			hoursMonth[iS].push_back(0);
			hoursWeeks[iS].push_back(std::array<int, 6>());
			hoursWeeksSlide[iS].push_back(std::vector<pair<int, int>>());
			impossiblePosts[iS].push_back(std::vector<int>());
			daySeq[iS].push_back(vector<vector<pair<int, int>>>());
			involved[iS].push_back(vector<vector<pair<pair<int, int>, pair<int, int>>>>());
			seqMinMax[iS].push_back(vector<vector<pair<int, int>>>());

			for (auto c : s->getConstraints()) {
				if (typeid(*c) == typeid(ConstraintDaysSeq)) {
					daySeq[iS][iA].push_back(vector<pair<int, int>>());
				}
				else if (typeid(*c) == typeid(ConstraintInvolved)) {
					involved[iS][iA].push_back(vector<pair<pair<int, int>, pair<int, int>>>());
				}
				else if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
					seqMinMax[iS][iA].push_back(vector<pair<int, int>>());
					if (((ConstraintSeqMinMax*)c)->getType() == MinMax::Min)
						score -= ((ConstraintSeqMinMax*)c)->getPriority();
				}
			}
			iA++;
		}
		iS++;
	}


	Valuation* v = new Valuation();

	v->setScore(score);
	v->setHoursMonth(hoursMonth);
	v->sethoursWeekSlide(hoursWeeksSlide);
	v->setImpossiblePosts(impossiblePosts);
	v->setDaySeq(daySeq);
	v->setInvolved(involved);
	v->setSeqMinMax(seqMinMax);
	v->setPostsRequirement(postRequirements);

	valuation = v;
}

//! Add a SwapLog to the list
//! \param swapLog SwapLog to add to the list
void Model::addSwapLog(const SwapLog swapLog){
	this->swapLog.push_back(swapLog);
}

//! \return swaplog vector of swaplog
const vector<SwapLog>* Model::getSwapLog() const {
	return &swapLog;
}

//! clear the swapLog vector
void Model::resetSwapLog(){
	swapLog.clear();
}

//! Undo lasts changes (swaps)
void Model::rollBack()
{
	for (int i = swapLog.size() - 1; i >= 0; i--) {
		auto p1 = agents[services[swapLog[i].getService1()]][swapLog[i].getAgent1()]->getCalendar()[swapLog[i].getDay()];
		agents[services[swapLog[i].getService1()]][swapLog[i].getAgent1()]->setCalendarDay(agents[services[swapLog[i].getService2()]]
			[swapLog[i].getAgent2()]->getCalendar()[swapLog[i].getDay()], swapLog[i].getDay());
		agents[services[swapLog[i].getService2()]][swapLog[i].getAgent2()]->setCalendarDay(p1, swapLog[i].getDay());
	}

	for (unsigned int i = 0; i < swapLog.size();i++) {
		swapLog[i].reverse();
	}
}

//! return predefinned constraints (used for CHIC)
vector<Constraint*> Model::createConstraints() {
	std::vector<Constraint*> constraints;

	//Pas de Nuit/Jour
	auto v = vector<int>();
	v.push_back(4);
	v.push_back(2);
	constraints.push_back(new ConstraintDaysSeq(v, 100)); // cJN

	//Pas 3 jours/nuit de travail d'affilée
	v = vector<int>();
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	constraints.push_back(new ConstraintDaysSeq(v, 150)); // c3N

	//Pas de nuit avant un congé posé
	v = vector<int>();
	v.push_back(4);
	v.push_back(6);
	constraints.push_back(new ConstraintDaysSeq(v, 50)); // crn

	//Après 2 jours/nuits au moins 2 repos
	v = vector<int>();
	v.push_back(1);
	v.push_back(1);
	auto v2 = vector<int>();
	v2.push_back(5);
	v2.push_back(5);
	constraints.push_back(new ConstraintInvolved(v, v2, -1, 50)); // cnr

	//Après 1 nuit longue + 1 repos  -> +1 repos min
	v = vector<int>();
	v.push_back(4);
	v.push_back(5);
	v2 = vector<int>();
	v2.push_back(5);
	constraints.push_back(new ConstraintInvolved(v, v2, -1, 100)); //cwl2r

	//Evite les journées isolées
	v = vector<int>();
	v.push_back(5);
	v.push_back(0);
	v.push_back(5);
	constraints.push_back(new ConstraintDaysSeq(v, 10)); //cji

	//1 week-end par mois
	v = vector<int>();
	v.push_back(0);
	v.push_back(0);
	constraints.push_back(new ConstraintSeqMinMax(5, MinMax::Min, 2, v, 1000));

	return constraints;
}

//! Add constraint returned by createConstraints() in the given Service
void Model::addBasicConstraintsTo(Service* s) {
	vector<Constraint*> cs = this->createConstraints();
	for (auto cons : cs){
		s->addConstraint(cons);
	}
}

Model Model::generateModelInstance(int firstDay, int nbDays, float overtime, int nbServices, int nbPosts, int nbAgents, float nbHoursWeek, float nbHoursMonth, int nbAgentsPerService, int nbPostsPerService, int proba_1er_conge, int proba_suite_conge) {

	//Si nbPostsPerService est indiqué, le nombre nbPosts ne sera pas respecté si nbPosts < nbPostsPerService*nbServices
	//de même pour nbAgentsPerService


	Model m = Model(firstDay, nbDays, overtime);
	m.defaultPost = NULL;

	int nbPostsAvailable = nbPosts, nbPostsService_i = -1, testLengthPost = -1, dayOrNight = -1;

	//Services et Postes

	//poste congé/repos dispos dans chaque service
	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut(5);

	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut(5);
	ca->addAttribut(6);

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
				nbPostsService_i = rand() % (nbPosts / (nbServices / 2)); //nbPosts entre 0 (2 en réalité) et nb total des posts/nbServices pour essayer d'avoir quelque chose d'équilibré
			}
			else { //si c'est le dernier service : lui attribuer tous les posts restants
				nbPostsService_i = nbPostsAvailable; //risque d'avoir bcp + de posts dans le dernier service
			}
		}

		cout << "Service " << i << " :" << nbPostsService_i << "poste(s)" << endl;

		if (nbPostsService_i < 2) {
			nbPostsService_i = 2; //empêche d'avoir des services sans postes mais => non respect de la valeur nbPosts des fois
		}

		//pour chaque post qui sera dans ce service
		for (int j = 0; j < nbPostsService_i; j++) {
			testLengthPost = rand() % 10 + 1;
			Post* posti_j;
			if (testLengthPost >= 5) { //50% de chance de créer un poste à 12.25h, sinon 7.5h

				posti_j = new Post("S" + std::to_string(i) + "P" + std::to_string(j) + "L", 12.25);
				posti_j->addAttribut(0);
				posti_j->addAttribut(1); //poste à durée longue
			}
			else {
				posti_j = new Post("S" + std::to_string(i) + "P" + std::to_string(j), 7.5);
				posti_j->addAttribut(0);
			}

			dayOrNight = rand() % 3 + 1; // 2 chances /3 d'être un poste en journée
			if (dayOrNight <= 2) {
				posti_j->setId(posti_j->getId() + "D");
				posti_j->addAttribut(2);
			}
			else {
				posti_j->setId(posti_j->getId() + "N");
				posti_j->addAttribut(4);
			}
			service_i->addPost(posti_j);
			service_i->addPostRequired(posti_j, 1);
		}

		service_i->addPost(repos); //chaque service a un poste Repos
		//et on ajoute à chaque service la liste des contraintes
		for (unsigned int id_cstr = 0; id_cstr < constraints.size(); id_cstr++) {
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
		service_rand = rand() % nbServices;
		if (nbAgentsPerService == -1) {
			m.addAgent(a_i, m.getServices()[service_rand]);
		}
		// sinon, ajout de l'agent au premier service libre
		else {
			int j = 0;
			while (m.getServices()[j]->getAgents().size() >= nbAgentsPerService) {
				j++;
			}
			m.addAgent(a_i, m.getServices()[j]);
		}

		vector<Post*> posts_possibles = m.getServices()[service_rand]->getPosts();

		//pour chaque jour
		for (int jour = 0; jour < nbDays; jour++) {
			conges_rand = rand() % 100 + 1;
			job_rand = rand() % 100 + 1;
			//cout << i << ": " << conges_rand << endl;
			if (conges_rand <= proba_1er_conge) { // proba_1er_conge% de chance d'avoir un premier congé ce jour ci

				a_i->setCalendarDay(ca, jour, true);

				//chances de faire une suite de congés après un premier congé = 70% puis décrément de 5% par jour à la suite
				conges_rand = rand() % 100 + 1;

				while (conges_rand <= (proba_suite_conge - (incr_jour * 5)) && jour + incr_jour <= nbDays) {
					a_i->setCalendarDay(ca, jour + incr_jour, true);
					conges_rand = rand() % 100 + 1;
					//cout << conges_rand << " : " << (70 - (incr_jour * 5) ) << endl;
					incr_jour++; //permet aussi le décrément de 5% de chance par jour succéssif
				}
				jour += incr_jour; //si on a déjà ajouté un congé aux jours suivants : on passe
				incr_jour = 0;
			}
			else if (job_rand <= 5) { //5% chance d'avoir un job pré-défini ce jour là
				post_ind_rand = rand() % posts_possibles.size();
				//cout << post_ind_rand << "  " << posts_possibles.size() << endl;
				if (posts_possibles[post_ind_rand]->getId() != "Repos") {
					a_i->setCalendarDay(posts_possibles[post_ind_rand], jour, true);
				}
			}
		}
	}

	return m;
} 

//! create a XML file from the Model
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
	defaultP->append_attribute(doc.allocate_attribute("timeAcc", doc.allocate_string(to_string(defaultPost->getTimeAccounted()).c_str())));

	//Attributs du post
	for (auto attri : defaultPost->getAttributs()) {
		xml_node<>* att = doc.allocate_node(node_element, "Attribut");
		att->append_attribute(doc.allocate_attribute("Attribut", doc.allocate_string(attributs[attri].c_str())));
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
			post->append_attribute(doc.allocate_attribute("timeAcc", doc.allocate_string(to_string(p->getTimeAccounted()).c_str())));

			//Attributs du post
			for (auto a : p->getAttributs()) {
				xml_node<>* att = doc.allocate_node(node_element, "Attribut");
				att->append_attribute(doc.allocate_attribute("Attribut", doc.allocate_string(attributs[a].c_str())));
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
				for (auto sA : *((ConstraintDaysSeq*)c)->getSequenceAtt()) {
					xml_node<>* att = doc.allocate_node(node_element, "Attribut");
					att->append_attribute(doc.allocate_attribute("Att", doc.allocate_string(attributs[sA].c_str())));
					constraint->append_node(att);
				}
				service->append_node(constraint);
			}
			else if (typeid(*c) == typeid(ConstraintInvolved)) {
				xml_node<>* constraint = doc.allocate_node(node_element, "ConstraintInvolved");
				constraint->append_attribute(doc.allocate_attribute("priority", doc.allocate_string(to_string(((ConstraintInvolved*)c)->getPriority()).c_str())));
				constraint->append_attribute(doc.allocate_attribute("day", doc.allocate_string(to_string(((ConstraintInvolved*)c)->getFirstDay()).c_str())));
				for (auto fs : *((ConstraintInvolved*)c)->getFirstSeqAtt()) {
					xml_node<>* att = doc.allocate_node(node_element, "FirstAttribut");
					att->append_attribute(doc.allocate_attribute("Att", doc.allocate_string(attributs[fs].c_str())));
					constraint->append_node(att);
				}
				for (auto fs : *((ConstraintInvolved*)c)->getLastSeqAtt()) {
					xml_node<>* att = doc.allocate_node(node_element, "LastAttribut");
					att->append_attribute(doc.allocate_attribute("Att", doc.allocate_string(attributs[fs].c_str())));
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
				for (auto fs : *((ConstraintSeqMinMax*)c)->getSequenceAtt()) {
					xml_node<>* att = doc.allocate_node(node_element, "Attribut");
					att->append_attribute(doc.allocate_attribute("Att", doc.allocate_string(attributs[fs].c_str())));
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
			agent->append_attribute(doc.allocate_attribute("nbHoursMonthPriority", doc.allocate_string(to_string(a->getNbHoursMonthPriority()).c_str())));
			agent->append_attribute(doc.allocate_attribute("nbHoursWeekPriority", doc.allocate_string(to_string(a->getNbHoursWeekPriority()).c_str())));
			agent->append_attribute(doc.allocate_attribute("impossiblePostPriority", doc.allocate_string(to_string(a->getImpossiblePostsPriority()).c_str())));


			//ImpossiblePosts
			for (auto ip : a->getImpossiblePosts()) {
				xml_node<>* post = doc.allocate_node(node_element, "ImpossiblePost");
				post->append_attribute(doc.allocate_attribute("id", doc.allocate_string(ip->getId().c_str())));
				post->append_attribute(doc.allocate_attribute("nbh", doc.allocate_string(to_string(ip->getTime()).c_str())));
				post->append_attribute(doc.allocate_attribute("timeAcc", doc.allocate_string(to_string(ip->getTimeAccounted()).c_str())));

				//Attributs du post
				for (auto attri : ip->getAttributs()) {
					xml_node<>* att = doc.allocate_node(node_element, "Attribut");
					att->append_attribute(doc.allocate_attribute("Attribut", doc.allocate_string(attributs[attri].c_str())));
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
						att->append_attribute(doc.allocate_attribute("Attribut", doc.allocate_string(attributs[attri].c_str())));
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

	firstDay = atoi(root->first_attribute("firstDay")->value());
	nbDays = atoi(root->first_attribute("nbDays")->value());
	overtime = atof(root->first_attribute("overtime")->value());

	//DefaultPost
	xml_node<>* defaultPost = root->first_node("DefaultPost");
	Post* post = new Post(defaultPost->first_attribute("id")->value(), atof(defaultPost->first_attribute("nbh")->value()), atoi(defaultPost->first_attribute("timeAcc")->value()));
	//attributs
	for (auto attNode = defaultPost->first_node("Attribut"); attNode; attNode = attNode->next_sibling("Attribut")) {
		if (find(attributs.begin(), attributs.end(), attNode->first_attribute("Attribut")->value()) == attributs.end())
			addAttribut(attNode->first_attribute("Attribut")->value());
		post->addAttribut(attributToInt(attNode->first_attribute("Attribut")->value()));
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

			//S'il n'y est pas, on le crée
			if (search == mapPosts.end()) {
				Post* post = new Post(postNode->first_attribute("id")->value(), atof(postNode->first_attribute("time")->value()), atoi(postNode->first_attribute("timeAcc")->value()));
				//attributs
				for (auto attNode = postNode->first_node("Attribut"); attNode; attNode = attNode->next_sibling("Attribut")) {
					if (find(attributs.begin(), attributs.end(), attNode->first_attribute("Attribut")->value()) == attributs.end())
						addAttribut(attNode->first_attribute("Attribut")->value());
					post->addAttribut(attributToInt(attNode->first_attribute("Attribut")->value()));
				}
				service->addPost(post);
				mapPosts.insert(pair<string, Post*>(post->getId(), post));
			}
			//S'il y est
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
			auto v = vector<int>();
			for (auto att = constraintDaySeqNode->first_node("Attribut"); att; att = att->next_sibling("Attribut")) {
				if (find(attributs.begin(), attributs.end(), att->first_attribute("Att")->value()) == attributs.end())
					addAttribut(att->first_attribute("Att")->value());
				v.push_back(attributToInt(att->first_attribute("Att")->value()));
			}
			service->addConstraint(new ConstraintDaysSeq(v, atoi(constraintDaySeqNode->first_attribute("priority")->value())));
		}

		for (auto constraintInvolved = serviceNode->first_node("ConstraintInvolved"); constraintInvolved; constraintInvolved = constraintInvolved->next_sibling("ConstraintInvolved")) {
			auto v = vector<int>();
			auto v2 = vector<int>();
			for (auto att = constraintInvolved->first_node("FirstAttribut"); att; att = att->next_sibling("FirstAttribut")) {
				if (find(attributs.begin(), attributs.end(), att->first_attribute("Att")->value()) == attributs.end())
					addAttribut(att->first_attribute("Att")->value());
				v.push_back(attributToInt(att->first_attribute("Att")->value()));
			}
			for (auto att = constraintInvolved->first_node("LastAttribut"); att; att = att->next_sibling("LastAttribut")) {
				if (find(attributs.begin(), attributs.end(), att->first_attribute("Att")->value()) == attributs.end())
					addAttribut(att->first_attribute("Att")->value());
				v2.push_back(attributToInt(att->first_attribute("Att")->value()));
			}
			service->addConstraint(new ConstraintInvolved(v, v2, atoi(constraintInvolved->first_attribute("day")->value()), atoi(constraintInvolved->first_attribute("priority")->value())));
		}

		for (auto constraintSeqMinMax = serviceNode->first_node("ConstraintSeqMinMax"); constraintSeqMinMax; constraintSeqMinMax = constraintSeqMinMax->next_sibling("ConstraintSeqMinMax")) {
			auto v = vector<int>();
			for (auto att = constraintSeqMinMax->first_node("Attribut"); att; att = att->next_sibling("Attribut")) {
				if (find(attributs.begin(), attributs.end(), att->first_attribute("Att")->value()) == attributs.end())
					addAttribut(att->first_attribute("Att")->value());
				v.push_back(attributToInt(att->first_attribute("Att")->value()));
			}
			service->addConstraint(new ConstraintSeqMinMax(atoi(constraintSeqMinMax->first_attribute("day")->value()), (MinMax)atoi(constraintSeqMinMax->first_attribute("type")->value()),
				atoi(constraintSeqMinMax->first_attribute("number")->value()), v, atoi(constraintSeqMinMax->first_attribute("priority")->value())));
		}

		//Agents
		for (auto agentNode = serviceNode->first_node("Agent"); agentNode; agentNode = agentNode->next_sibling("Agent")) {
			auto agent = new Agent(agentNode->first_attribute("id")->value(), atoi(agentNode->first_attribute("nbHoursMonth")->value()),
				atoi(agentNode->first_attribute("nbHoursWeek")->value()), (Status)atoi(agentNode->first_attribute("status")->value()));
			agent->setNbHoursMonthPriority(atoi(agentNode->first_attribute("nbHoursMonthPriority")->value()));
			agent->setNbHoursWeekPriority(atoi(agentNode->first_attribute("nbHoursWeekPriority")->value()));
			agent->setImpossiblePostsPriority(atoi(agentNode->first_attribute("impossiblePostPriority")->value()));
			//Calendrier
			int i = 0;
			for (auto dayNode = agentNode->first_node("Day"); dayNode; dayNode = dayNode->next_sibling("Day")) {
				if ((string)dayNode->first_attribute("id")->value() != "null") {
					auto search = mapPosts.find(dayNode->first_attribute("id")->value());

					//S'il n'y est pas, on le crée
					if (search == mapPosts.end()) {
						Post* post = new Post(dayNode->first_attribute("id")->value(), atof(dayNode->first_attribute("nbh")->value()), atoi(dayNode->first_attribute("timeAcc")->value()));
						//attributs
						for (auto attNode = dayNode->first_node("Attribut"); attNode; attNode = attNode->next_sibling("Attribut")) {
							if (find(attributs.begin(), attributs.end(), attNode->first_attribute("Attribut")->value()) == attributs.end())
								addAttribut(attNode->first_attribute("Attribut")->value());
							post->addAttribut(attributToInt(attNode->first_attribute("Attribut")->value()));
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
				//On check s'il existe déjà
				auto search = mapPosts.find(impossiblePost->first_attribute("id")->value());

				//S'il n'y est pas, on le crée
				if (search == mapPosts.end()) {
					Post* post = new Post(impossiblePost->first_attribute("id")->value(), atof(impossiblePost->first_attribute("nbh")->value()), atoi(impossiblePost->first_attribute("timeAcc")->value()));
					//attributs
					for (auto attNode = impossiblePost->first_node("Attribut"); attNode; attNode = attNode->next_sibling("Attribut")) {
						if (find(attributs.begin(), attributs.end(), attNode->first_attribute("Attribut")->value()) == attributs.end())
							addAttribut(attNode->first_attribute("Attribut")->value());
						post->addAttribut(attributToInt(attNode->first_attribute("Attribut")->value()));
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

//! Generate a XML file with the planning
//! \param fileName name of the file to create
void Model::generateXMLPlanning(string fileName)
{
	ofstream res;
	// A RECUPERER DE L'INSTANCE
	string mois = "mois";
	string line = " ";

	res.open(fileName);

	res << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	res << "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\nxmlns:o=\"urn:schemas-microsoft-com:office:office\"\nxmlns:x=\"urn:schemas-microsoft-com:office:excel\"\nxmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\nxmlns:html=\"http://www.w3.org/TR/REC-html40\">";
	res << "<Styles>";
	// Style pour repos : case blanche
	res << "<Style ss:ID=\"Default\" ss:Name=\"Repos\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>\n<Interior ss:Color=\"#FFFFFF\" ss:Pattern=\"Solid\"/>\n</Style>";
	// Style pour w-e : case rose pale
	res << "<Style ss:ID=\"we\" ss:Name=\"Week-End\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>\n<Interior ss:Color=\"#F7D9F7\" ss:Pattern=\"Solid\"/>\n</Style>";
	// Style pour CA : case gris foncé
	//res << "<Style ss:ID=\"ca\" ss:Name=\"Congé\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>\n<Interior ss:Color=\"#888888\" ss:Pattern=\"Solid\"/>\n</Style>";
	// Style pour les dates+jours : case rouge
	res << "<Style ss:ID=\"setup\" ss:Name=\"Setup\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\" ss:Bold=\"1\"/>\n<Interior ss:Color=\"#FF0000\" ss:Pattern=\"Solid\"/>\n</Style>";
	// Style pour les jours travaillés : case gris clair
	res << "<Style ss:ID=\"work\" ss:Name=\"Travail\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>\n<Interior ss:Color=\"#E0E0E0\" ss:Pattern=\"Solid\"/>\n</Style>";
	
	res << "</Styles>\n";
	res << "<Worksheet ss:Name =\"" << mois << "\">";
	res << "<Table>";

	
	for (auto service : services) {
		res << "<Row>\n";
		res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">" << service->getId() << "</Data></Cell>\n";
		res << "</Row\n>";
		res << "<Row>\n";
		res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">Dates</Data></Cell>\n";
		// RECUPERER NB JOURS DU MOIS
		for (int i = 1; i <= nbDays; i++) {
			res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"Number\">" << i << "</Data></Cell>\n";
		}
		res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">" << "nbHours" << "</Data></Cell>\n";
		res << "</Row>\n";

		res << "<Row>\n";
		res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">Jours</Data></Cell>\n";

		auto currentDay = firstDay;
		for (int i = 0; i < nbDays; i++) {
			res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">";
			switch (currentDay) {
			case 0:
				res << "M";
				break;
			case 1:
				res << "T";
				break;
			case 2:
				res << "W";
				break;
			case 3:
				res << "Th";
				break;
			case 4:
				res << "F";
				break;
			case 5:
				res << "Sat";
				break;
			case 6:
				res << "Su";
				break;
			default:
				res << "";
				break;
			}
			res << "</Data></Cell>\n";

			currentDay = (currentDay + 1) % 7;
		}
		res << "</Row>\n";

		// RECUPERER LE NOMBRE D'AGENTS
		auto lAgents = agents[service];
		for (auto agent : lAgents) {
			res << "<Row>\n";
			res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">" << "Agent " << agent->getId() << "</Data></Cell>\n";

			for (int i = 0; i < nbDays; i++) {
				res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">";
				auto p = agent->getCalendar()[i];
				if (p != NULL)
					res << p->getId();
				else
					res << " ";

				res << "</Data></Cell>\n";
			}
			res << "<Cell ss:StyleID = \"Default\"><Data ss:Type=\"Number\">" << agent->getWorkingHoursMonth(getFirstDay(), true) << "</Data></Cell>\n";
			res << "</Row>\n";
		}
		res << "<Row>\n</Row>\n";
		res << "<Row>\n</Row>\n";
	}

	res << "</Table>";
	res << "</Worksheet>";
	res << "</Workbook>";

	res.close();
}

void Model::addAttribut(string att)
{
	attributs.push_back(att);
}

int Model::attributToInt(string att)
{
	for (int i = 0; i < attributs.size(); i++) {
		if (attributs[i] == att)
			return i;
	}
	return -1;
}

//! \return constraintsInformation informations about what constraints are broken, work only if valuation is filled
string Model::getConstraintInformations()
{
	string s;

	for (int i = 0; i < services.size(); i++) {
		s += "Service " + services[i]->getId() + "\n";

			for (unsigned int j = 0; j < valuation->getDaySeq()[i].size(); j++) {
				for (unsigned int k = 0; k < valuation->getDaySeq()[i][j].size(); k++) {
					for (unsigned int l = 0; l < valuation->getDaySeq()[i][j][k].size(); l++) {
						s += "ConstraintDaySeq: Service " + to_string(i) + " Agent " + to_string(j) + " constraint " + to_string(k) + " Jours "
							+ to_string(valuation->getDaySeq()[i][j][k][l].first) + " " + to_string(valuation->getDaySeq()[i][j][k][l].second) + "\n";
					}
				}
			}

			for (unsigned int j = 0; j < valuation->getInvolved()[i].size(); j++) {
				for (unsigned int k = 0; k < valuation->getInvolved()[i][j].size(); k++) {
					for (unsigned int l = 0; l < valuation->getInvolved()[i][j][k].size(); l++) {
						s += "ConstraintInvolved: Service " + to_string(i) + " Agent " + to_string(j) + " constraint " + to_string(k) + " Jours "
							+ to_string(valuation->getInvolved()[i][j][k][l].first.first) + "-" + to_string(valuation->getInvolved()[i][j][k][l].first.second)
							+ " " + to_string(valuation->getInvolved()[i][j][k][l].second.first) + "-" + to_string(valuation->getInvolved()[i][j][k][l].second.second) + "\n" ;
					}
				}
			}

			for (unsigned int j = 0; j < valuation->getseqMinMax()[i].size(); j++) {
				for (unsigned int k = 0; k < valuation->getseqMinMax()[i][j].size(); k++) {
					for (unsigned int l = 0; l < valuation->getseqMinMax()[i][j][k].size(); l++) {
						s += "ConstraintseqMinMax: Service " + to_string(i) + " Agent " + to_string(j) + " constraint " + to_string(k) + " Jours "
							+ to_string(valuation->getseqMinMax()[i][j][k][l].first) + " " + to_string(valuation->getseqMinMax()[i][j][k][l].second) + "\n";
					}
				}
			}

			for (unsigned int j = 0; j < 31; j++) {
				if (valuation->getPostsRequirement()[i][j] > 0)
					s += "PostRequirements: Service " + to_string(i) + " Day " + to_string(j) + " nbFail " + to_string(valuation->getPostsRequirement()[i][j]) + "\n";
			}

			for (unsigned int j = 0; j < valuation->getHoursMonth()[i].size(); j++) {
				s += "HoursMonth: Service " + to_string(i) + " Agent " + to_string(j) + " -> " + to_string(valuation->getHoursMonth()[i][j]) + "\n";
			}

			for (unsigned int j = 0; j < valuation->gethoursWeekSlide()[i].size(); j++) {
				for (unsigned int k = 0; k < valuation->gethoursWeekSlide()[i][j].size(); k++) {
					s += "hoursWeekSlide: Service " + to_string(i) + " Agent " + to_string(j) + " Day " + to_string(valuation->gethoursWeekSlide()[i][j][k].first) + " to " + to_string(valuation->gethoursWeekSlide()[i][j][k].second) + "\n";
				}
			}

			for (unsigned int j = 0; j < valuation->getImpossiblePosts()[i].size(); j++) {
				for (unsigned int k = 0; k < valuation->getImpossiblePosts()[i][j].size(); k++) {
					s += "impossiblePosts: Service " + to_string(i) + " Agent " + to_string(j) + " Day " + to_string(valuation->getImpossiblePosts()[i][j][k]) + "\n";
				}
			}

			s += "\n\n";
		}

	return s;

}
