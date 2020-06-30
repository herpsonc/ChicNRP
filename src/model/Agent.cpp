/*
 * Agent.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Agent.h"

Agent::Agent() {
	id ="";
	// service = NULL;
	nbHoursMonth = 0;
	nbHoursWeek = 0;
	status = Confirmed;
}

Agent::Agent(string id, int nbHourMonth, int nbHoursWeek, Status status)
{
	this->id = id;
	// service = NULL;
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
	// this->service=obj.service;
	this->nbHoursMonth=obj.nbHoursMonth;
	this->nbHoursWeek=obj.nbHoursWeek;
	this->status=obj.status;
	this->calendar=obj.calendar;
	this->lastMonthCalendar = obj.lastMonthCalendar;
	this->impossiblePosts=obj.impossiblePosts;
	// this->constraints=obj.constraints;
	this->calendarLock = obj.calendarLock;
}

Agent::~Agent() {
}

//! \return id id of the Agent
const string& Agent::getId() const {
	return id;
}

//! \param id id to set
void Agent::setId(const string &id) {
	this->id = id;
}

//! \return nbHoursMonth number of hours in the month accepted for the Agent
float Agent::getNbHoursMonth() const {
	return nbHoursMonth;
}

//! \param nbHours number of hours to set for the month
void Agent::setNbHoursMonth(float nbHours) {
	this->nbHoursMonth = nbHours;
}

//! \return calendar calendar of the Agent where each index is a Post for a day(Also the solution for the Agent)
const array<Post*, 31>& Agent::getCalendar() const {
	return calendar;
}

//! \return calendarLock a calendar where each day is a bool, used to lock some Post in algorithms 
const array<bool, 31>& Agent::getCalendarLock() const {
	return calendarLock;
}

//! \param calendar calendar to set
void Agent::setCalendar(const array<Post*, 31> &calendar) {
	this->calendar = calendar;
}

//! \return impossiblePosts vector of Post not supposed to be affected to the Agent
const std::vector<Post*>& Agent::getImpossiblePosts() const {
	return impossiblePosts;
}

//! \param impossiblePosts impossibles Post to set
void Agent::setImpossiblePosts(const std::vector<Post*> &impossiblePosts) {
	this->impossiblePosts = impossiblePosts;
}

//! \return service service affected to the Agent
/*Service*& Agent::getService() {
	return service;
}

//! \return constraints vector of Constraint of the Agent (Not used)
const std::vector<Constraint*>& Agent::getConstraints() const {
	return constraints;
}

//! \param constraints Constraint to set
void Agent::setConstraints(const std::vector<Constraint*> &constraints) {
	this->constraints = constraints;
}

//! \param service Service to set
void Agent::setService(Service *&service) {
	this->service = service;
}

//! Add a Constraint to the Agent
//! \param constraint Constraint to add
void Agent::addConstraint(Constraint *constraint) {
	constraints.push_back(constraint);
}*/

//! \return status status of the Agent
Status Agent::getStatus() const {
	return status;
}

//! \param status status to set
void Agent::setStatus(Status status) {
	this->status = status;
}

//! \return lastMonthCalendar calendar of the 7 days before the month start
const array<Post*, 7>& Agent::getLastMonthCalendar() const {
	return lastMonthCalendar;
}

//! \param lastMonthCalendar last month calendar to set
void Agent::setLastMonthCalendar(const array<Post*, 7> &lastMonthCalendar) {
	this->lastMonthCalendar = lastMonthCalendar;
}

//! Set a day of the calendar, the day of the calendarLock is set to false by default
//! \param post Post to set
//! \param day day at which the post is setted
void Agent::setCalendarDay(Post *post, int day) {
	if(day>=0 && day<31)
	{
		calendar[day] = post;
		calendarLock[day] = false;
	}
}

//! Set a day of the calendar
//! \param post Post to set
//! \param day day at which the post is setted
//! \param lock true to lock the day
void Agent::setCalendarDay(Post* post, int day, bool lock) {
	if (day >= 0 && day < 31)
	{
		calendar[day] = post;
		calendarLock[day] = lock;
	}
}

//! \return nbHoursWeek number of hours in a week (7days) accepted for the Agent
float Agent::getNbHoursWeek() const {
	return nbHoursWeek;
}

//! \param number of hours to set
void Agent::setNbHoursWeek(float nbHoursWeek) {
	this->nbHoursWeek = nbHoursWeek;
}

//! \return r number of hours worked in the month
float Agent::getWorkingHoursMonth() {
	float r = 0;
	for(auto day : calendar){
		if(day != NULL)
			r += day->getTime();
	}

	return r;
}

//! \param log true to print log on console
//! \return nbFail number of fail (+1 everytime there is more than nbHoursWeek in 7 consecutive days)
int Agent::checkWorkingHoursWeek(bool log){

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

//! Faster version of checkWorkingHoursWeek
//! \param val Valuation of the current Model
//! \param idService index(on the Model) of the Service
//! \param day this func only check around this day
//! \param idA index(on the Model) of the Agent
void Agent::checkWorkingHoursWeekFast(Valuation*val, int idService, int day, int idA){
	auto v = std::vector<std::pair<int, int>>();
	int cptHours = 0;
	int nbFail = 0;
	for (int i = day - 6; i < day + 7; i++) {
		cptHours = 0;

		for (int j = 0; j < 7; j++) {
			if (i + j >= 0 && i + j < 31 && calendar[i + j] != NULL) {
				cptHours += calendar[i + j]->getTime();
			}
		}
		if (cptHours > nbHoursWeek) {
			nbFail++;
			v.push_back(std::pair<int, int>(i, i + 6));
		}
	}

	//Update la valuation
	auto valuation = val->gethoursWeekSlide()[idService][idA];
	auto newVec = vector<pair<int, int>>();
	bool found = false;
	for (auto value : valuation) {
		//Si la contrainte est dans l'intervalle, on vérifie qu'elle est toujours active
		if (value.first >= day - 6 && value.second <= day + 7) {
			found = false;
			for (auto e : v) {
				if (value.first == e.first && value.second == e.second) {
					newVec.push_back(value);
					found = true;
				}
			}
			//Si on le trouve pas, c'est qu'on a résolu la contrainte
			if (!found) {
				val->setScore(val->getScore() + 1);
			}
		}
		else {
			newVec.push_back(value);
		}
	}
	//Ajout des nouveaux éléments
	for (auto e : v) {
		bool isIn = false;
		for (auto value : valuation) {
			if (value.first == e.first && value.second == e.second) {
				isIn = true;
			}
		}

		if (!isIn) {
			newVec.push_back(e);
			val->setScore(val->getScore() - 1);
		}
	}

	auto vecToAdd = val->gethoursWeekSlide();
	vecToAdd[idService][idA] = newVec;
	val->sethoursWeekSlide(vecToAdd);

}

//! Used to build the valuation of the Model from 0
//! return v vector of intervals where this constraint is broken
std::vector<std::pair<int, int>> Agent::checkWorkingHoursWeekValuation(){
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

//! \param true to print log on the console
//! \return nbFail +1 everytime an impossible Post is detected in the calendar
int Agent::checkImpossiblePosts(bool log){
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

//! Faster version of checkImpossiblePosts
//! \param val Valuation of the current Model
//! \param idService index(on the Model) of the Service
//! \param day day to check
//! \param idA index(on the Model) of the Agent
void Agent::checkImpossiblePostsFast(Valuation* val, int idService, int day, int idA){
	
	bool fail = false;
	for (auto ip : impossiblePosts) {
		if (calendar[day] == ip) {
			fail = true;
		}
	}

	bool found = false;
	auto newVec = vector<int>();
	//Check dans Valuation
	auto value = val->getImpossiblePosts()[idService][idA];
	for (auto e : value) {
		if (e == day) {
			found = true;
			if (fail)
				newVec.push_back(e);
			else
				val->setScore(val->getScore() + 10);
		}
		else {
			newVec.push_back(e);
		}
	}
	if (!found && fail) {
		newVec.push_back(day);
		val->setScore(val->getScore() - 10);
	}

	auto vec = val->getImpossiblePosts();
	vec[idService][idA] = newVec;
	val->setImpossiblePosts(vec);
}

//! Used to build the valuation of the Model from 0
//! \return vec vector of each day where this constraint is broken
std::vector<int> Agent::checkImpossiblePostsValuation(){
	auto vec = vector<int>();
	int nbFail = 0;
	int i = 0;
	for (auto p : calendar) {
		for (auto ip : impossiblePosts) {
			if (p == ip) {
				vec.push_back(i);
			}

		}
		i++;
	}

	return vec;
}

