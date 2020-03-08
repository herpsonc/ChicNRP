/*
 * Model.h
 *
 *  Created on: 1 mars 2020
 *      Author: Valars
 */

enum Day {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday, None};

#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

#include <iostream>
#include <vector>
#include "Post.h"
#include "Service.h"
#include "Agent.h"

class Model {
public:
	Model(Day firstDay, int nbDays, float overtime);
	virtual ~Model();

	const std::vector<Agent*>& getAgents() const;
	const std::vector<Service*>& getServices() const;

	void addAgent(Agent* agent);
	void addService(Service* service);

	void printPlanning();

protected:
	Day firstDay;
	int nbDays;
	float overtime; //heure supp (Delta)
	std::vector<Agent*> agents;
	std::vector<Service*> services;
	std::vector<Post*> posts;



};

#endif /* SRC_MODEL_MODEL_H_ */
