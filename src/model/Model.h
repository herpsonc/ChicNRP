/*
 * Model.h
 *
 *  Created on: 1 mars 2020
 *      Author: Valars
 */
#pragma once

enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday, None };

#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

#include <iostream>
#include <vector>
#include <set>
#include <stdio.h>
#include <fstream>
#include "Post.h"
#include "Service.h"
#include "Agent.h"
#include "Valuation.h"
#include "SwapLog.h"

#include "constraint/Constraint.h"
#include "constraint/ConstraintDaysSeq.h"
#include "constraint/ConstraintInvolved.h"
#include "constraint/ConstraintSeqMinMax.h"

#include "../rapidXml/rapidxml.hpp"
#include "../rapidXml/rapidxml_utils.hpp"



class Model {
public:
	Model(Day firstDay, int nbDays, float overtime);
	Model(const Model& obj);
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
	Valuation* getValuation();
	void setValuation(Valuation v);

	void addSwapLog(const SwapLog swapLog);
	std::vector<SwapLog> getSwapLog();
	void resetSwapLog();

	std::vector<Constraint*> Model::createConstraints();
	static Model generateModelInstance(Day firstDay, int nbDays, float overtime, int nbServices, int nbPosts, int nbAgents, float nbHoursWeek, float nbHoursMonth, int nbAgentsPerService = -1, int nbPostsPerService = -1, int proba_1er_conge = -1, int proba_suite_conge = -1);

	void generateXML(string fileName);
	void loadXML(string fileName);

protected:
	Day firstDay;
	int nbDays;
	float overtime; //heure supp (Delta)
	Post* defaultPost;
	std::map<Service*,std::vector<Agent*>> agents;
	std::vector<Service*> services;
	std::vector<Post*> posts;
	Valuation* valuation;
	std::vector<SwapLog> swapLog;



};

#endif /* SRC_MODEL_MODEL_H_ */
