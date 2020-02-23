/*
 * Service.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Service.h"
#include "Agent.H"

Service::Service() {
	id = "";
	posts = vector<Post>();
	referents = vector<Agent>();

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

const vector<Post>& Service::getPosts() const {
	return posts;
}

void Service::setPosts(const vector<Post> &posts) {
	this->posts = posts;
}

const vector<Agent>& Service::getReferents() const {
	return referents;
}

void Service::setReferents(const vector<Agent> &referents) {
	this->referents = referents;
}
