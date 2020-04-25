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
	nbHoursMonth = 0;
	nbHoursWeek = 0;
	status = Confirmed;
}

Agent::Agent(string id, int nbHourMonth, int nbHoursWeek, Status status)
{
	this->id = id;
	service = NULL;
	this->nbHoursMonth = nbHourMonth;
	this->nbHoursWeek = nbHoursWeek;
	this->status = status;

	for (int i = 0;i < calendar.size();i++)
		calendar[i] = NULL;

	for (int i = 0;i < calendarLock.size();i++)
		calendarLock[i] = false;

	for (int i = 0;i < lastMonthCalendar.size();i++)
		lastMonthCalendar[i] = NULL;
}

Agent::Agent(const Agent &obj){
	this->id=obj.id;
	this->service=obj.service;
	this->nbHoursMonth=obj.nbHoursMonth;
	this->nbHoursWeek=obj.nbHoursWeek;
	this->status=obj.status;
	this->calendar=obj.calendar;
	this->lastMonthCalendar = obj.lastMonthCalendar;
	this->impossiblePosts=obj.impossiblePosts;
	this->constraints=obj.constraints;
	this->calendarLock = obj.calendarLock;
}

Agent::~Agent() {
}

const string& Agent::getId() const {
	return id;
}

void Agent::setId(const string &id) {
	this->id = id;
}

float Agent::getNbHoursMonth() const {
	return nbHoursMonth;
}

void Agent::setNbHoursMonth(float nbHours) {
	this->nbHoursMonth = nbHours;
}

const array<Post*, 31>& Agent::getCalendar() const {
	return calendar;
}

const array<bool, 31>& Agent::getCalendarLock() const {
	return calendarLock;
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
		calendar[day] = post;
		calendarLock[day] = false;
	}
}

void Agent::setCalendarDay(Post* post, int day, bool lock) {
	if (day >= 0 && day < 31)
	{
		calendar[day] = post;
		calendarLock[day] = lock;
	}
}

float Agent::getNbHoursWeek() const {
	return nbHoursWeek;
}

void Agent::setNbHoursWeek(float nbHoursWeek) {
	this->nbHoursWeek = nbHoursWeek;
}

float Agent::getWorkingHoursMonth() {
	float r = 0;
	for(auto day : calendar){
		if(day != NULL)
			r += day->getTime();
	}

	return r;
}

float Agent::getWorkingHoursWeek(Day day, int weekI) {
	float r = 0;
	int indice = day;
	for(auto day : calendar){
		if(day != NULL && indice/7 == weekI){
			r+= day->getTime();
		}
		indice++;
	}
	return r;
}

int Agent::checkWorkingHoursWeek(bool log)
{

	int cptHours = 0;
	int nbFail = 0;
	for (int i = 0; i < 31; i++) {
		cptHours = 0;

		for (int j = 0; j < 7; j++) {
			if (i+j < 31 && calendar[i + j] != NULL) {
				cptHours += calendar[i + j]->getTime();
			}
		}
		if (cptHours > nbHoursWeek) {
			if(log)
				cout << "Agent " << id << " heure semaine: jours " << i << " a " << i + 6 << " heures: " << cptHours << endl;
			nbFail++;
		}
	}

	return nbFail;
}

std::vector<std::pair<int, int>> Agent::checkWorkingHoursWeekValuation()
{
	auto v = std::vector<std::pair<int, int>>();
	int cptHours = 0;
	int nbFail = 0;
	for (int i = 0; i < 31; i++) {
		cptHours = 0;

		for (int j = 0; j < 7; j++) {
			if (i + j < 31 && calendar[i + j] != NULL) {
				cptHours += calendar[i + j]->getTime();
			}
		}
		if (cptHours > nbHoursWeek) {
			nbFail++;
			v.push_back(std::pair<int, int>(i, i + 6));
		}
	}

	return v;
}

int Agent::checkImpossiblePosts(bool log)
{
	int nbFail = 0;
	int i = 0;
	for (auto p : calendar) {
		for (auto ip : impossiblePosts) {
			if (p == ip) {
				nbFail++;
				if (log) {
					cout << "Agent " << id << " impossible post " << ip->getId() << " jour " << i << endl;
				}
			}

		}
		i++;
	}

	return nbFail;
}

