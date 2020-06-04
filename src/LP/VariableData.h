/*Classe permettant d'avoir toute les informations liées à une SCIP_VAR */

#include "../model/Agent.h"
#include "../model/Post.h"

#include <scip/scip.h>
//#include <scip/scipdefplugins.h>

#ifndef SRC_LP_VARIABLEDATA_H_
#define SRC_LP_VARIABLEDATA_H_

class VariableData {

public:
	VariableData(SCIP_VAR* variable, int id, int day, Agent* agent,	Post* post);
	virtual ~VariableData();

	SCIP_VAR* getVariable();
	int getId();
	int getDay();
	Agent* getAgent();
	Post* getPost();

protected:
	int id;
	SCIP_VAR* variable;
	int day;
	Agent* agent;
	Post* post;
};

#endif

