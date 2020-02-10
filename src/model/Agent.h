/*
 * Agent.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include <string>
#include "Service.h"

using namespace std;

#ifndef MODEL_AGENT_H_
#define MODEL_AGENT_H_

class Agent {
public:
	Agent();
	virtual ~Agent();

	bool isBeginner() const;
	void setBeginner(bool beginner);
	const string& getId() const;
	void setId(const string &id);
	bool isInf6Month() const;
	void setInf6Month(bool inf6Month);
	int getNbHours() const;
	void setNbHours(int nbHours);



protected:
	string id;
	bool beginner;
	bool inf6Month;
	int nbHours;

	Service* service;

};

#endif /* MODEL_AGENT_H_ */
