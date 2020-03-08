/*
 * Post.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Post.h"

using namespace std;

Post::Post() {
	id ="";
	time = 0;
}

Post::Post(string id, float time)
{
	this->id = id;
	this->time = time;
}

Post::~Post() {
	// TODO Auto-generated destructor stub
}

const string& Post::getId() const {
	return id;
}

void Post::setId(const string &id) {
	this->id = id;
}

float Post::getTime() const {
	return time;
}

void Post::setTime(float time) {
	this->time = time;
}

const vector<Constraint*>& Post::getConstraints() const {
	return constraints;
}

void Post::setConstraints(const vector<Constraint*> &constraints) {
	this->constraints = constraints;
}

void Post::addConstraint(Constraint *constraint) {
	constraints.push_back(constraint);
}

const vector<string>& Post::getAttributs() const {
	return attributs;
}

void Post::setAttributs(const vector<string> &attributs) {
	this->attributs = attributs;
}

void Post::addAttribut(string attribut) {
	attributs.push_back(attribut);
}
