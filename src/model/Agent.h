/*
 * Agent.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include <string>
#include "Service.h"
#include "constraint/Constraint.h"
#include "Model.h"
#include "Valuation.h"

#ifndef MODEL_AGENT_H_
#define MODEL_AGENT_H_

enum Status {Beginner, Rooki, Confirmed};

class Agent {
public:
	Agent();
	Agent(string id, int nbHourMonth,int nbHoursWeek, Status status);
	Agent(const Agent &obj);
	virtual ~Agent();

	const string& getId() const;
	void setId(const string &id);

	float getNbHoursMonth() const;
	void setNbHoursMonth(float nbHours);

	const array<Post*, 31>& getCalendar() const;
	const array<bool, 31>& getCalendarLock() const;
	void setCalendar(const array<Post*, 31> &calendar);
	void setCalendarDay(Post* post, int day);
	void setCalendarDay(Post* post, int day, bool lock);

	const std::vector<Post*>& getImpossiblePosts() const;
	void setImpossiblePosts(const std::vector<Post*> &impossiblePosts);

	Service*& getService();
	void setService( Service *&service);

	const std::vector<Constraint*>& getConstraints() const;
	void setConstraints(const std::vector<Constraint*> &constraints);
	void addConstraint(Constraint* constraint);

	Status getStatus() const;
	void setStatus(Status status);
	const array<Post*, 7>& getLastMonthCalendar() const;
	void setLastMonthCalendar(const array<Post*, 7> &lastMonthCalendar);
	float getNbHoursWeek() const;
	void setNbHoursWeek(float nbHoursWeek);

	float getWorkingHoursMonth();

	int checkWorkingHoursWeek(bool log);
	void checkWorkingHoursWeekFast(Valuation* val, int idService, int day, int idA);
	std::vector<std::pair<int, int>> checkWorkingHoursWeekValuation();
	int checkImpossiblePosts(bool log);
	void checkImpossiblePostsFast(Valuation* val, int idService, int day, int idA);
	std::vector<int> checkImpossiblePostsValuation();

protected:
	string id;
	Status status;
	float nbHoursMonth;
	float nbHoursWeek;

	array<Post*, 31> calendar; //Liste où chaque élément = un jour (donc ordonné) avec le poste attribué
	array<bool, 31> calendarLock;
	array<Post*, 7> lastMonthCalendar; //Liste des 7 derniers postes avant le début du mois
	std::vector<Post*> impossiblePosts; // Liste des potes qui ne sont pas attribuables à l'agent (C4)

	std::vector<Constraint*> constraints; //Not used

	Service* service; //service préféré/affecté // Not used

};

#endif /* MODEL_AGENT_H_ */
