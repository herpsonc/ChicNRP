#include <string>
#include <array>
#include "Post.h"

#ifndef MODEL_AGENT_H_
#define MODEL_AGENT_H_

enum Status {Beginner, Rooki, Confirmed};

/*! \class
 * 	This class represent an agent with all his characteristics
 *
 */

class Agent {
public:
	Agent(string id, float nbHourMonth, float nbHoursWeek, Status status);
	Agent(const Agent &obj);
	virtual ~Agent();

	const string& getId() const;
	void setId(const string &id);

	float getNbHoursMonth() const;
	void setNbHoursMonth(float nbHours);

	const array<Post*, 31>& getCalendar() const;
	const array<bool, 31>& getCalendarLock() const;
	void setCalendar(const array<Post*, 31> &calendar);
	void setCalendarUnlockOnly(const array<Post*, 31>& calendar);
	void setCalendarDay(Post* post, int day, bool lock=false);

	const std::vector<Post*>& getImpossiblePosts() const;
	void setImpossiblePosts(const std::vector<Post*> &impossiblePosts);

	Status getStatus() const;
	void setStatus(Status status);
	const array<Post*, 7>& getLastMonthCalendar() const;
	void setLastMonthCalendar(const array<Post*, 7> &lastMonthCalendar);
	float getNbHoursWeek() const;
	void setNbHoursWeek(float nbHoursWeek);

	float getWorkingHoursMonth(int firstDay, bool accounted = false);

	void setNbHoursMonthPriority(int priority);
	int getNbHoursMonthPriority() const;

	void setNbHoursWeekPriority(int priority);
	int getNbHoursWeekPriority() const;

	void setImpossiblePostsPriority(int priority);
	int getImpossiblePostsPriority() const;

protected:
	string id;
	Status status;
	float nbHoursMonth;
	float nbHoursWeek;

	//Chaque élément de calendar correspond à un jour (donc ordonné) avec le poste attribué
	array<Post*, 31> calendar; 
	array<bool, 31> calendarLock;
	array<Post*, 7> lastMonthCalendar; //Liste des 7 derniers postes du mois précédent
	std::vector<Post*> impossiblePosts; // Liste des postes qui ne sont pas attribuables à l'agent (C4)

	int nbHoursMonthPriority;
	int nbHoursWeekPriority;
	int impossiblePostsPriority;

};

#endif /* MODEL_AGENT_H_ */
