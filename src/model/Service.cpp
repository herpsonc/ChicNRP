/*
 * Service.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Service.h"
#include "Agent.H"

Service::Service() {
	id = "";
	posts = vector<Post>();
	referents = vector<Agent>();

}

Service::~Service() {
	// TODO Auto-generated destructor stub
}

