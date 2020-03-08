/*
 * Agent.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Agent.h"

Agent::Agent() {
	id ="";
	service = NULL;
	nbHours = 0;
	status = Confirmed;
}

Agent::Agent(string id, int nbHour, Status status)
{
	this->id = id;
	service = NULL;
	this->nbHours = nbHour;
	this->status = status;
}

Agent::~Agent() {
}

const string& Agent::getId() const {
	return id;
}

void Agent::setId(const string &id) {
	this->id = id;
}


float Agent::getNbHours() const {
	return nbHours;
}

void Agent::setNbHours(float nbHours) {
	this->nbHours = nbHours;
}

const array<Post*, 31>& Agent::getCalendar() const {
	return calendar;
}

void Agent::setCalendar(const array<Post*, 31> &calendar) {
	this->calendar = calendar;
}

const std::vector<Post*>& Agent::getImpossiblePosts() const {
	return impossiblePosts;
}

void Agent::setImpossiblePosts(const std::vector<Post*> &impossiblePosts) {
	this->impossiblePosts = impossiblePosts;
}

Service*& Agent::getService() {
	return service;
}

const std::vector<Constraint*>& Agent::getConstraints() const {
	return constraints;
}

void Agent::setConstraints(const std::vector<Constraint*> &constraints) {
	this->constraints = constraints;
}

void Agent::setService(Service *&service) {
	this->service = service;
}

void Agent::addConstraint(Constraint *constraint) {
	constraints.push_back(constraint);
}

Status Agent::getStatus() const {
	return status;
}

void Agent::setStatus(Status status) {
	this->status = status;
}

const array<Post*, 7>& Agent::getLastMonthCalendar() const {
	return lastMonthCalendar;
}

void Agent::setLastMonthCalendar(const array<Post*, 7> &lastMonthCalendar) {
	this->lastMonthCalendar = lastMonthCalendar;
}

void Agent::setCalendarDay(Post *post, int day) {
	if(day>=0 && day<31)
	{
		calendar[day]=post;
	}
}
