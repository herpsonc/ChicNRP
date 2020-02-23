/*
 * Agent.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include <string>
#include "Service.h"

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

	std::vector<Post> calendar; //Liste où chaque élément = un jour (donc ordonné) avec le poste attribué
	std::vector<Post> impossiblePosts; // Liste des potes qui ne sont pas attribuables à l'agent (C4)

	Service* service;

};

#endif /* MODEL_AGENT_H_ */
