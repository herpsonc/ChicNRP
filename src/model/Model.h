#pragma once


#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

#include <iostream>
#include <set>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include "Service.h"
#include "SwapLog.h"

#include "constraint/ConstraintDaysSeq.h"
#include "constraint/ConstraintInvolved.h"
#include "constraint/ConstraintSeqMinMax.h"

#include "../rapidXml/rapidxml.hpp"
#include "../rapidXml/rapidxml_utils.hpp"
#include "../rapidXml/rapidxml_print.hpp"
#include "Valuation.h"



class Model {
public:
	Model(int firstDay, int nbDays, float overtime);
	Model(const Model& obj);
	virtual ~Model();

	Model& operator = (const Model& obj);

	std::vector<Service*>& getServices();

	const std::vector<Agent*> getAgentFrom(Service* service);
	const std::vector<Agent*>* getAgentFromPtr(Service* service);

	void addAgent(Agent* agent, Service* service);
	void addService(Service* service);

	void printPlanning();

	int getFirstDay() const;
	void setFirstDay(int firstDay);

	int getNbDays() const;
	void setNbDays(int nbDays);

	int idDayToDay(int idDay);

	float getOvertime() const;
	void setOvertime(float overtime);

	Post* getDefaultPost();
	void setDefaultPost(Post* defaultPost);

	std::vector<Post*>& getPosts();
	void addPost(Post*);

	Valuation* getValuation();
	void setValuation(Valuation v);
	void generateEmptyValuation();

	void addSwapLog(const SwapLog swapLog);
	const std::vector<SwapLog>* getSwapLog() const;
	void resetSwapLog();
	void rollBack();

	std::vector<Constraint*> createConstraints();
	void addBasicConstraintsTo(Service* s);
	static Model generateModelInstance(int firstDay, int nbDays, float overtime, int nbServices, int nbPosts, int nbAgents, float nbHoursWeek, float nbHoursMonth, int nbAgentsPerService = -1, int nbPostsPerService = -1, int proba_1er_conge = -1, int proba_suite_conge = -1);

	void generateXML(string fileName);
	void loadXML(string fileName);
	void generateXlsx(string fileName);


protected:
	int firstDay;
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
