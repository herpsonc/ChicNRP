/*
 * Agent.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include <string>
#include "Service.h"
#include "constraint\Constraint.h"

#ifndef MODEL_AGENT_H_
#define MODEL_AGENT_H_

enum Status {Beginner, Rooki, Confirmed};

class Agent {
public:
	Agent();
	Agent(string id, int nbHour, Status status);
	virtual ~Agent();

	const string& getId() const;
	void setId(const string &id);

	float getNbHours() const;
	void setNbHours(float nbHours);

	const array<Post*, 31>& getCalendar() const;
	void setCalendar(const array<Post*, 31> &calendar);
	void setCalendarDay(Post* post, int day);

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

protected:
	string id;
	Status status;
	float nbHours;

	array<Post*, 31> calendar; //Liste où chaque élément = un jour (donc ordonné) avec le poste attribué
	array<Post*, 7> lastMonthCalendar; //Liste des 7 derniers postes avant le début du mois
	std::vector<Post*> impossiblePosts; // Liste des potes qui ne sont pas attribuables à l'agent (C4)

	std::vector<Constraint*> constraints;

	Service* service; //service préféré/affecté

};

#endif /* MODEL_AGENT_H_ */
