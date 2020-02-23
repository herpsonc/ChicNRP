/*
 * Post.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Post.h"

Post::Post() {
	id ="";
	time = 0;
}

Post::~Post() {
	// TODO Auto-generated destructor stub
}

const string& Post::getId() const {
	return id;
}

void Post::setId(const string &id) {
	this->id = id;
}

int Post::getTime() const {
	return time;
}

void Post::setTime(int time) {
	this->time = time;
}
