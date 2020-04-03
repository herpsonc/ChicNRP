/*
 * Model.h
 *
 *  Created on: 1 mars 2020
 *      Author: Valars
 */
#pragma once

enum Day {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday, None};

#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

#include <iostream>
#include <vector>
#include <set>
#include "Post.h"
#include "Service.h"
#include "Agent.h"

class Model {
public:
	Model(Day firstDay, int nbDays, float overtime);
	Model(const Model &obj);
	virtual ~Model();

	Model& operator = (const Model& obj);

	const std::set<Service*>& getServices() const;

	std::vector<Agent*> getAgentFrom(Service* service);

	void addAgent(Agent* agent, Service* service);
	void addService(Service* service);

	void printPlanning();

	Day getFirstDay() const;
	void setFirstDay(Day firstDay);
	static Day getNextDay(Day day);

	int getNbDays() const;
	void setNbDays(int nbDays);
	float getOvertime() const;
	void setOvertime(float overtime);
	Post* getDefaultPost();
	void setDefaultPost(Post *defaultPost);

protected:
	Day firstDay;
	int nbDays;
	float overtime; //heure supp (Delta)
	Post* defaultPost;
	std::map<Service*,std::vector<Agent*>> agents;
	std::set<Service*> services;
	std::vector<Post*> posts;



};

#endif /* SRC_MODEL_MODEL_H_ */
