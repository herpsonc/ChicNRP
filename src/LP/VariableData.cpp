/*Classe permettant d'avoir toute les informations liées à une SCIP_VAR */

#include "VariableData.h"
#include <iostream>

using namespace std;

VariableData::VariableData(SCIP_VAR* variable, int id, int day, Agent* agent, Post* post) {
	this->id = id;
	this->variable = variable;
	this->day = day;
	this->agent = agent;
	this->post = post;
}

VariableData::~VariableData() {
	std::cout << "VariableData destroyed";
}

int VariableData::getId(){
	return id;
}

SCIP_VAR* VariableData::getVariable() {
	return variable;
}

int VariableData::getDay() {
	return day;
}

Agent* VariableData::getAgent() {
	return agent;
}

Post* VariableData::getPost() {
	return post;
}

