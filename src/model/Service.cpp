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
	// TODO Auto-generated destructor stub
}

const string& Service::getId() const {
	return id;
}

void Service::setId(const string &id) {
	this->id = id;
}

const vector<Post*>& Service::getPosts() const {
	return posts;
}

void Service::setPosts(const vector<Post*> &posts) {
	this->posts = posts;
}

const vector<Agent*>& Service::getReferents() const {
	return referents;
}

void Service::setReferents(const vector<Agent*> &referents) {
	this->referents = referents;
}


void Service::addPost(Post* post) {
	posts.push_back(post);
}

void Service::addReferent(Agent* referent) {
	referents.push_back(referent);
}

const vector<Constraint*>& Service::getConstraints() const {
	return constraints;
}

void Service::setConstraints(const vector<Constraint*> &constraints) {
	this->constraints = constraints;
}

void Service::addConstraint(Constraint *constraint) {
	constraints.push_back(constraint);
}

const array<map<Post*, int>, 7>& Service::getPostRequired() const {
	return postRequired;
}

void Service::setPostRequired(const array<map<Post*, int>, 7> &postRequired) {
	this->postRequired = postRequired;
}

void Service::addPostRequired(Post *post, int nb) {
	for(int i=0;i<7;i++){
		postRequired[i][post] = nb;
	}
}

void Service::addPostRequired(Post *post, int nb, int day) {
	postRequired[day][post] = nb;
}
