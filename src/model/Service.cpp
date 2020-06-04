/*
 * Service.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Service.h"
#include "Agent.H"

using namespace std;

Service::Service() {
	id = "";
	posts = vector<Post*>();
	referents = vector<Agent*>();

}

Service::Service(string id) {
	this->id = id;
}

Service::~Service() {
}

//! \return id id of the Service
const string& Service::getId() const {
	return id;
}

//! \param id id to set
void Service::setId(const string &id) {
	this->id = id;
}

//! \return posts vector of Post that are attributed to the service 
const vector<Post*>& Service::getPosts() const {
	return posts;
}

//! \param posts posts to set
void Service::setPosts(const vector<Post*> &posts) {
	this->posts = posts;
}

//! \return referents vector of referent Agent of the Service 
const vector<Agent*>& Service::getReferents() const {
	return referents;
}

//! \param referents referents to set
void Service::setReferents(const vector<Agent*> &referents) {
	this->referents = referents;
}

//! \param post post to add
void Service::addPost(Post* post) {
	posts.push_back(post);
}

//! \param referent referent to add
void Service::addReferent(Agent* referent) {
	referents.push_back(referent);
}

//! \return constraints vector of Constraint of the Service
const vector<Constraint*>& Service::getConstraints() const {
	return constraints;
}

//! \param constraints constraint to set
void Service::setConstraints(const vector<Constraint*> &constraints) {
	this->constraints = constraints;
}

//! \param constraint Constraint to add
void Service::addConstraint(Constraint *constraint) {
	constraints.push_back(constraint);
}

//! \return postRequired vector of the required posts for each days of the week
const array<map<Post*, int>, 7>& Service::getPostRequired() const {
	return postRequired;
}

//! \param postRequired post required to set
void Service::setPostRequired(const array<map<Post*, int>, 7> &postRequired) {
	this->postRequired = postRequired;
}

//! Add a required Post for each days
//! \param post Post to add
//! \param nb number required for a day
void Service::addPostRequired(Post *post, int nb) {
	for(int i=0;i<7;i++){
		postRequired[i][post] = nb;
	}
}

//! Add a required Post for the given day
//! \param post Post to add
//! \param nb number required for a day
//! \param day day where the Post is added
void Service::addPostRequired(Post *post, int nb, int day) {
	postRequired[day][post] = nb;
}

//! \return agents vector of Agent of the Service
const vector<Agent*>& Service::getAgents() const {
	return agents;
}

//! \param agents agents to set
void Service::setAgents(const vector<Agent*> &agents) {
	this->agents = agents;
}

//! \param agent Agent to add
void Service::addAgent(Agent *agent) {
	agents.push_back(agent);
}
