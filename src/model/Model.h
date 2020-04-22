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
#include "../../Valuation.h"

class Model {
public:
	Model(Day firstDay, int nbDays, float overtime);
	Model(const Model &obj);
	virtual ~Model();

	Model& operator = (const Model& obj);

	std::vector<Service*>& getServices();

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

	void setDefaultPost(Post* defaultPost);
	const Valuation& getValuation();
	void setValuation(const Valuation v);

	static Model generateModelInstance(Day firstDay, int nbDays, float overtime, int nbServices, int nbPosts, int nbAgents, float nbHoursWeek, int nbAgentsPerService = -1, int nbPostsPerService = -1);


protected:
	Day firstDay;
	int nbDays;
	float overtime; //heure supp (Delta)
	Post* defaultPost;
	std::map<Service*,std::vector<Agent*>> agents;
	std::vector<Service*> services;
	std::vector<Post*> posts;
	Valuation valuation;



};

#endif /* SRC_MODEL_MODEL_H_ */
