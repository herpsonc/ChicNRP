﻿#include "Agent.h"
#include <iostream>

using namespace std;


Agent::Agent(string id, float nbHourMonth, float nbHoursWeek, Status status)
{
	this->id = id;
	this->nbHoursMonth = nbHourMonth;
	this->nbHoursWeek = nbHoursWeek;
	this->status = status;
	this->impossiblePosts = vector<Post*>();

	for (int i = 0;i < (int)calendar.size();i++)
		calendar[i] = NULL;

	for (int i = 0;i < (int)calendarLock.size();i++)
		calendarLock[i] = false;

	for (int i = 0;i < (int)lastMonthCalendar.size();i++)
		lastMonthCalendar[i] = NULL;

	nbHoursMonthPriority = 10;
	nbHoursWeekPriority = 150;
	impossiblePostsPriority = 100;
}

Agent::Agent(const Agent &obj){
	this->id=obj.id;
	this->nbHoursMonth=obj.nbHoursMonth;
	this->nbHoursWeek=obj.nbHoursWeek;
	this->status=obj.status;
	this->calendar=obj.calendar;
	this->lastMonthCalendar = obj.lastMonthCalendar;
	this->impossiblePosts=obj.impossiblePosts;
	this->calendarLock = obj.calendarLock;

	this->nbHoursMonthPriority = obj.nbHoursMonthPriority;
	this->nbHoursWeekPriority = obj.nbHoursWeekPriority;
	this->impossiblePostsPriority = obj.impossiblePostsPriority;
}

Agent::~Agent() {
}

//! \return id of the Agent
const string& Agent::getId() const {
	return id;
}

//! \param id id to set
void Agent::setId(const string &id) {
	this->id = id;
}

//! \return number of hours in the month accepted for the Agent
float Agent::getNbHoursMonth() const {
	return nbHoursMonth;
}

//! \param nbHours number of hours to set for the month
void Agent::setNbHoursMonth(float nbHours) {
	this->nbHoursMonth = nbHours;
}

//! \return calendar of the Agent where each index is a Post for a day(Also the solution for the Agent)
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

void Agent::setCalendarUnlockOnly(const array<Post*, 31>& calendar)
{
	for (unsigned int i = 0; i < calendar.size(); i++) {
		if (!calendarLock[i])
			this->calendar[i] = calendar[i];
	}
}

//! \return vector of Post not supposed to be affected to the Agent
const std::vector<Post*>& Agent::getImpossiblePosts() const {
	return impossiblePosts;
}

//! \param impossiblePosts impossibles Post to set
void Agent::setImpossiblePosts(const std::vector<Post*> &impossiblePosts) {
	this->impossiblePosts = impossiblePosts;
}

//! \return status of the Agent
Status Agent::getStatus() const {
	return status;
}

//! \param status status to set
void Agent::setStatus(Status status) {
	this->status = status;
}

//! \return calendar of the 7 days before the month start
const array<Post*, 7>& Agent::getLastMonthCalendar() const {
	return lastMonthCalendar;
}

//! \param lastMonthCalendar last month calendar to set
void Agent::setLastMonthCalendar(const array<Post*, 7> &lastMonthCalendar) {
	this->lastMonthCalendar = lastMonthCalendar;
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

//! \return number of hours in a week (7days) accepted for the Agent
float Agent::getNbHoursWeek() const {
	return nbHoursWeek;
}

//! \param number of hours to set
void Agent::setNbHoursWeek(float nbHoursWeek) {
	this->nbHoursWeek = nbHoursWeek;
}

//! \return number of hours worked in the month
float Agent::getWorkingHoursMonth(int firstDay, bool accounted)
{
	float r = 0;
	for (auto day : calendar) {
		if (day != NULL && (day->getTimeAccounted() || (!accounted && firstDay < 5)))
			r += day->getTime();

		firstDay = (firstDay + 1) % 7;
	}

	return r;
}


void Agent::setNbHoursMonthPriority(int priority)
{
	nbHoursMonthPriority = priority;
}

int Agent::getNbHoursMonthPriority() const
{
	return nbHoursMonthPriority;
}

void Agent::setNbHoursWeekPriority(int priority)
{
	nbHoursWeekPriority = priority;
}

int Agent::getNbHoursWeekPriority() const
{
	return nbHoursWeekPriority;
}

void Agent::setImpossiblePostsPriority(int priority)
{
	impossiblePostsPriority = priority;
}

int Agent::getImpossiblePostsPriority() const
{
	return impossiblePostsPriority;
}


