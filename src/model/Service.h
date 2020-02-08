/*
 * Service.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include <string>
#include <vector>
#include "Post.h"


using namespace std;

#ifndef MODEL_SERVICE_H_
#define MODEL_SERVICE_H_

class Agent;

class Service {
public:
	Service();
	virtual ~Service();

protected:
	string id;
	vector<Post> posts;
	vector<Agent> referents;
};

#endif /* MODEL_SERVICE_H_ */
